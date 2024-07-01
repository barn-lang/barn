/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2024
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
#include <barn_struct.h>

static barn_type_t* barn_type_u8_global;
static barn_type_t* barn_type_u16_global;
static barn_type_t* barn_type_u32_global;
static barn_type_t* barn_type_u64_global;
static barn_type_t* barn_type_i8_global;
static barn_type_t* barn_type_i16_global;
static barn_type_t* barn_type_i32_global;
static barn_type_t* barn_type_i64_global;
static barn_type_t* barn_type_ptr_global;
static barn_type_t* barn_type_str_global;
static barn_type_t* barn_type_bool_global;
static barn_type_t* barn_type_f32_global;
static barn_type_t* barn_type_f64_global;

void
barn_initalize_types()
{
    barn_type_u8_global   = barn_create_type(BARN_TYPE_U8);
    barn_type_u16_global  = barn_create_type(BARN_TYPE_U16);
    barn_type_u32_global  = barn_create_type(BARN_TYPE_U32);
    barn_type_u64_global  = barn_create_type(BARN_TYPE_U64);
    barn_type_i8_global   = barn_create_type(BARN_TYPE_I8);
    barn_type_i16_global  = barn_create_type(BARN_TYPE_I16);
    barn_type_i32_global  = barn_create_type(BARN_TYPE_I32);
    barn_type_i64_global  = barn_create_type(BARN_TYPE_I64);
    barn_type_ptr_global  = barn_create_type(BARN_TYPE_PTR);
    barn_type_str_global  = barn_create_type(BARN_TYPE_STRING);
    barn_type_bool_global = barn_create_type(BARN_TYPE_BOOL);
    barn_type_f32_global  = barn_create_type(BARN_TYPE_F32);
    barn_type_f64_global  = barn_create_type(BARN_TYPE_F64);
}

barn_type_t* 
barn_create_type(barn_type_kind_t type)
{
    barn_type_t* new_type = (barn_type_t*)calloc(1, sizeof(barn_type_t));

    new_type->type = type;
    new_type->size = type == BARN_TYPE_FORMAT ? 0 : barn_convert_type_to_size(type);

    new_type->is_unsigned = barn_is_type_unsigned(type);
    new_type->is_signed   = barn_is_type_signed(type);
    new_type->is_string   = barn_is_type_string(type);
    new_type->is_struct   = barn_is_type_struct(type);
    new_type->is_float    = barn_is_type_float(type);
    new_type->is_bool     = barn_is_type_bool(type);
    new_type->is_ptr      = barn_is_type_ptr(type);
    new_type->is_any      = barn_is_type_any(type);

    if (new_type->is_struct)
        barn_error_show(BARN_COMPILER_ERROR, "to create an struct type use barn_struct_type_create function");

    return new_type;
}

bool
barn_is_type_any(barn_type_kind_t type)
{
    return (type == BARN_TYPE_ANY);
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
barn_is_type_struct(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_STRUCT));
}

bool 
barn_is_type_string(barn_type_kind_t type)
{
    return (type == BARN_TYPE_STRING);
}

bool
barn_is_type_number(barn_type_kind_t type)
{
    return ((barn_is_type_signed(type) || barn_is_type_unsigned(type) || 
             barn_is_type_float (type) || barn_is_type_bool    (type))
                ? true
                : false);
}

bool 
barn_is_type_float(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_F32) || 
            (type == BARN_TYPE_F64));
}

bool
barn_is_type_bool(barn_type_kind_t type)
{
    return ((type == BARN_TYPE_BOOL));
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
        case BARN_TYPE_ANY:
        case BARN_TYPE_NONE:
        case BARN_TYPE_AUTO:
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

barn_type_t*
barn_parser_current_token_type_representation(barn_parser_t* parser)
{
    barn_type_t* type = NULL;

    if      (BARN_STR_CMP(parser->curr_token->value, "u8") || BARN_STR_CMP(parser->curr_token->value, "byte"))
        type = barn_type_u8_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "u16") || BARN_STR_CMP(parser->curr_token->value, "ushort"))
        type = barn_type_u16_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "u32") || BARN_STR_CMP(parser->curr_token->value, "uint"))
        type = barn_type_u32_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "u64") || BARN_STR_CMP(parser->curr_token->value, "ulong"))
        type = barn_type_u64_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "i8") || BARN_STR_CMP(parser->curr_token->value, "char"))
        type = barn_type_i8_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "i16") || BARN_STR_CMP(parser->curr_token->value, "short"))
        type = barn_type_i16_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "i32") || BARN_STR_CMP(parser->curr_token->value, "int"))
        type = barn_type_i32_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "i64") || BARN_STR_CMP(parser->curr_token->value, "long"))
        type = barn_type_i64_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "f32") || BARN_STR_CMP(parser->curr_token->value, "float"))
        type = barn_type_f32_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "f64") || BARN_STR_CMP(parser->curr_token->value, "double"))
        type = barn_type_f64_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "string"))
        type = barn_type_str_global;
    else if (BARN_STR_CMP(parser->curr_token->value, "bool"))
        type = barn_type_bool_global;
    else
        type = barn_parser_struct_find_by_name(parser, parser->curr_token->value);

    // TODO: implement pointers
    return type;
}

const char*
barn_convert_type_to_string(barn_type_t* type)
{
    switch (type->type)
    {
        case BARN_TYPE_AUTO:
            return "auto";
            break;
        case BARN_TYPE_U8:
            return "u8";
            break;
        case BARN_TYPE_I8:
            return "i8";
            break;
        case BARN_TYPE_BOOL:
            return "bool";
            break;
        case BARN_TYPE_NONE:
            return "none";
            break;
        case BARN_TYPE_U16:
            return "u16";
            break;
        case BARN_TYPE_I16:
            return "i16";
            break;
        case BARN_TYPE_U32:
            return "u32";
            break;
        case BARN_TYPE_I32:
            return "i32";
            break;
        case BARN_TYPE_F32:
            return "f32";
            break;
        case BARN_TYPE_U64:
            return "u64";
            break;
        case BARN_TYPE_I64:
            return "i64";
            break;
        case BARN_TYPE_F64:
            return "f64";
            break;
        case BARN_TYPE_PTR:
            return "ptr*";
            break;
        case BARN_TYPE_STRING:
            return "string";
            break;
        case BARN_TYPE_FORMAT:
            return "format";
            break;
        case BARN_TYPE_ANY:
            return "any";
            break;
        case BARN_TYPE_STRUCT:
            return type->structure.sturct_type_name;
            break;
        default:
            BARN_UNIMPLEMENTED("unhandled type size");
            break;        
    }

    return "unknown";
}

barn_type_t* barn_get_type_u8_global()   
{ 
    return barn_type_u8_global; 
}

barn_type_t* barn_get_type_u16_global()  
{ 
    return barn_type_u16_global;
}

barn_type_t* barn_get_type_u32_global()  
{ 
    return barn_type_u32_global;
}

barn_type_t* barn_get_type_u64_global()  
{ 
    return barn_type_u64_global;
}

barn_type_t* barn_get_type_i8_global()   
{ 
    return barn_type_i8_global; 
}

barn_type_t* barn_get_type_i16_global()  
{ 
    return barn_type_i16_global;
}

barn_type_t* barn_get_type_i32_global()  
{ 
    return barn_type_i32_global;
}

barn_type_t* barn_get_type_i64_global()  
{ 
    return barn_type_i64_global;
}

barn_type_t* barn_get_type_ptr_global()  
{ 
    return barn_type_ptr_global;
}

barn_type_t* barn_get_type_str_global()  
{ 
    return barn_type_str_global;
}

barn_type_t* barn_get_type_bool_global() 
{ 
    return barn_type_bool_global;
}

barn_type_t* barn_get_type_f32_global()  
{ 
    return barn_type_f32_global;
}

barn_type_t* barn_get_type_f64_global()  
{ 
    return barn_type_f64_global;
}