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

#ifndef __BARN_STRUCT__
#define __BARN_STRUCT__

#include <barn_core.h>
#include <barn_parser.h>
#include <barn_types.h>

typedef struct __barn_struct_field_t {
    const char*  field_name;
    barn_type_t* field_type;
} barn_struct_field_t;

size_t barn_struct_calculate_size(barn_type_t* type);

barn_type_t* barn_struct_type_create(char* name);
barn_type_t* barn_parser_struct_find_by_name(barn_parser_t* parser, char* struct_name);

bool barn_parser_is_structure_defined(barn_parser_t* parser, char* struct_name);
bool barn_parser_is_structure_field_named(barn_parser_t* parser, barn_type_t* structure, char* field_name);

void barn_parser_struct(barn_parser_t* parser);

barn_struct_field_t* barn_parser_get_structure_field_named(barn_parser_t* parser, barn_type_t* structure, char* field_name);
barn_struct_field_t* barn_struct_create_new_field(barn_type_t* struct_type, char* name, barn_type_t* type);

#endif /* __BARN_STRUCT__ */