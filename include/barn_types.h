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

#ifndef __BARN_TYPES__
#define __BARN_TYPES__

#include <barn_core.h>

typedef enum __barn_type_kind_t {
    /* Unsigned */
    BARN_TYPE_U8,
    BARN_TYPE_U16,
    BARN_TYPE_U32,
    BARN_TYPE_U64,

    /* Signed */
    BARN_TYPE_I8,
    BARN_TYPE_I16,
    BARN_TYPE_I32,
    BARN_TYPE_I64,

    /* Other */
    BARN_TYPE_PTR,
    BARN_TYPE_STRING,
    BARN_TYPE_BOOL,

    /* Float */
    BARN_TYPE_F32,
    BARN_TYPE_F64,

    /* Other */
    BARN_TYPE_STRUCT
} barn_type_kind_t;

typedef struct __barn_type_t {
    barn_type_kind_t type;
    size_t size;

    bool is_unsgined;
    bool is_signed;
    bool is_string;
    bool is_float;
    bool is_ptr;

    // TODO: structure fields
} barn_type_t;

barn_type_t* barn_create_type(barn_type_kind_t type);

size_t barn_convert_type_to_size(barn_type_kind_t type);

bool barn_is_type_unsigned(barn_type_kind_t type);
bool barn_is_type_signed(barn_type_kind_t type);
bool barn_is_type_string(barn_type_kind_t type);
bool barn_is_type_float(barn_type_kind_t type);
bool barn_is_type_ptr(barn_type_kind_t type);

#endif /* __BARN_TYPES__ */