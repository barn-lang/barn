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

#ifndef __BARN_VARIABLES__
#define __BARN_VARIABLES__

#include <barn_core.h>
#include <barn_types.h>

typedef struct __barn_variable_t {
    bool         is_const;
    bool         is_used;
    barn_type_t* var_type;
    const char*  var_name;
} barn_variable_t;
 
barn_variable_t* barn_create_variable(const char* var_name, barn_type_t* var_type, 
                                      bool is_const, bool is_used);

void barn_parser_variable_declaration(barn_parser_t* parser);

bool barn_parser_is_variable_defined_lg(barn_parser_t* parser, const char* variable_name);
bool barn_parser_is_variable_defined_l (barn_parser_t* parser, const char* variable_name);
bool barn_parser_is_variable_defined_g (barn_parser_t* parser, const char* variable_name);

barn_variable_t* barn_parser_get_variable_by_name(barn_parser_t* parser, const char* variable_name);

#endif /* __BARN_VARIABLES__ */