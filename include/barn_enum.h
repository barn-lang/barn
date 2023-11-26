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

#ifndef __BARN_ENUM__
#define __BARN_ENUM__

#include <barn_core.h>
#include <barn_parser.h>

typedef struct __barn_enum_field_t {
    const char*  enum_name;
    barn_node_t* enum_expression;
} barn_enum_field_t;

void barn_parser_enum(barn_parser_t* parser);

barn_enum_field_t* barn_create_enum_field(const char* enum_name, barn_node_t* enum_expression);

#endif /* __BARN_ENUM__ */