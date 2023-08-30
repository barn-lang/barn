/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2023
 *   
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
*/

#include <barn_core.h>

#include <barn_types.h>
#include <barn_parser.h>

barn_type_t* 
barn_create_type(barn_type_kind_t type)
{
    barn_type_t* new_type = (barn_type_t*)calloc(1, sizeof(barn_type_t));

    new_type->type = type;
    new_type->size = barn_convert_type_to_size(type);

    new_type->is_unsgined = barn_is_type_unsigned(type);
    new_type->is_signed   = barn_is_type_signed(type);
    new_type->is_string   = barn_is_type_string(type);
    new_type->is_float    = barn_is_type_float(type);
    new_type->is_ptr      = barn_is_type_ptr(type);

    return new_type;
}

bool 
barn_is_type_unsigned(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_U8)  || 
            (type == BARN_TYPE_U16) || 
            (type == BARN_TYPE_U32) || 
            (type == BARN_TYPE_U64));
}

bool 
barn_is_type_signed(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_I8)  || 
            (type == BARN_TYPE_I16) || 
            (type == BARN_TYPE_I32) || 
            (type == BARN_TYPE_I64));
}

bool 
barn_is_type_string(barn_type_kind_t type)
{
    return (type == BARN_TYPE_STRING);
}

bool 
barn_is_type_float(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_F32) || 
            (type == BARN_TYPE_F64));
}

bool 
barn_is_type_ptr(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_PTR)   || 
            (type == BARN_TYPE_STRING));
}

size_t 
barn_convert_type_to_size(barn_type_kind_t type)
{
    switch (type)
    {
        case BARN_TYPE_U8:
        case BARN_TYPE_I8:
        case BARN_TYPE_BOOL:
            return 1;
            break;
        case BARN_TYPE_U16:
        case BARN_TYPE_I16:
            return 2;
            break;
        case BARN_TYPE_U32:
        case BARN_TYPE_I32:
        case BARN_TYPE_F32:
            return 4;
            break;
        case BARN_TYPE_U64:
        case BARN_TYPE_I64:
        case BARN_TYPE_F64:
        case BARN_TYPE_PTR:
        case BARN_TYPE_STRING:
            return 8;
            break;
        default:
            BARN_UNIMPLEMENTED("unhandled type size");
            break;
    }

    return -1;
}
