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

#ifndef __BARN_TYPES__
#define __BARN_TYPES__

#include <barn_core.h>
#include <barn_parser.h>

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
    BARN_TYPE_STRUCT,
    BARN_TYPE_ANY,
    BARN_TYPE_NONE,
    BARN_TYPE_FORMAT,
    BARN_TYPE_AUTO
} barn_type_kind_t;

typedef struct __barn_type_t {
    barn_type_kind_t type;
    size_t size;

    bool is_unsigned;
    bool is_signed;
    bool is_string;
    bool is_struct;
    bool is_float;
    bool is_bool; 
    bool is_char;
    bool is_ptr;
    bool is_any;

    union {
        barn_array_t* struct_fields;
        const char*   sturct_type_name;
    } structure;
} barn_type_t;

barn_type_t* barn_create_type(barn_type_kind_t type);

size_t barn_convert_type_to_size(barn_type_kind_t type);
const char* barn_convert_type_to_string(barn_type_t* type);

bool barn_is_type_unsigned(barn_type_kind_t type);
bool barn_is_type_signed(barn_type_kind_t type);
bool barn_is_type_string(barn_type_kind_t type);
bool barn_is_type_struct(barn_type_kind_t type);
bool barn_is_type_float(barn_type_kind_t type);
bool barn_is_type_bool(barn_type_kind_t type);
bool barn_is_type_ptr(barn_type_kind_t type);
bool barn_is_type_any(barn_type_kind_t type);

bool barn_is_type_number(barn_type_kind_t type);

barn_type_t* barn_parser_current_token_type_representation(barn_parser_t* parser);

void barn_initalize_types();

barn_type_t* barn_get_type_u8_global();
barn_type_t* barn_get_type_u16_global();
barn_type_t* barn_get_type_u32_global();
barn_type_t* barn_get_type_u64_global();
barn_type_t* barn_get_type_i8_global();
barn_type_t* barn_get_type_i16_global();
barn_type_t* barn_get_type_i32_global();
barn_type_t* barn_get_type_i64_global();
barn_type_t* barn_get_type_ptr_global();
barn_type_t* barn_get_type_str_global();
barn_type_t* barn_get_type_bool_global();
barn_type_t* barn_get_type_f32_global();
barn_type_t* barn_get_type_f64_global();

#endif /* __BARN_TYPES__ */