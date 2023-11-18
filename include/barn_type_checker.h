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

#ifndef __BARN_TYPE_CHECKER__
#define __BARN_TYPE_CHECKER__

#include <barn_core.h>
#include <barn_parser.h>
#include <barn_nodes.h>

typedef struct __barn_type_checker_t {
    barn_array_t*  nodes;
    barn_parser_t* parser;
    barn_node_t*   curr_node;
    
    int index;
} barn_type_checker_t;

void barn_type_checker_start(barn_parser_t* parser);

bool barn_tc_does_types_collide_expr_value(barn_expression_value_t* lhs, barn_expression_value_t* rhs);
bool barn_tc_does_types_collides(barn_type_t* lhs, barn_type_t* rhs);

void barn_tc_func_return_check(barn_type_checker_t* tc, barn_node_t* return_node);
void barn_tc_func_call_check(barn_type_checker_t* tc, barn_node_t* call_node);

barn_type_t* barn_tc_expression_get_type(barn_type_checker_t* tc, barn_node_t* expr_node);

#endif /* __BARN_TYPE_CHECKER__ */  