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

#ifndef __BARN_FUNCTIONS__
#define __BARN_FUNCTIONS__

#include <barn_core.h>
#include <barn_parser.h>

#define BARN_FUNCTION_INJECTING_CODE "__code__"
#define BARN_FUNCTION_USE_CODE       "__use__"

typedef struct __barn_func_argument_t {
    barn_type_t* argument_type;
    const char*  argument_name;
} barn_func_argument_t;

typedef struct __barn_parse_function_args_t {
    bool expect_name, expect_comma, expect_type;
    size_t index;
} barn_parse_function_args_t;

/* 
 * This function named `barn_parser_collect_function_name`
 * is a very interesting one because it first of all checks
 * is the current token EOF kind next we duplicate this string
 * and checks is it a correct namespace and does a function
 * like this doesn't exists already 
 */
const char* barn_parser_collect_function_name(barn_parser_t* parser);

void barn_parser_extern_function_declaration(barn_parser_t* parser);
void barn_parser_function_declaration(barn_parser_t* parser);

barn_node_t* barn_parser_function_get_by_name(barn_parser_t* parser, char* function_name);

barn_func_argument_t* barn_create_func_argument(barn_type_t* argument_type, const char* argument_name);

void barn_initialize_builtin_functions();

bool barn_parser_function_is_argument_list_with_format(barn_array_t* function_args);

#endif /* __BARN_FUNCTIONS__ */