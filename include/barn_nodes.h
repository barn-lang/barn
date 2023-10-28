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

#ifndef __BARN_NODES__
#define __BARN_NODES__

typedef struct __barn_node_t barn_node_t;
typedef struct __barn_type_t barn_type_t;
typedef struct __barn_variable_t barn_variable_t;

#include <barn_core.h>
#include <barn_array.h>
#include <barn_types.h>
#include <barn_expressions.h>
#include <barn_variables.h>

typedef enum __barn_node_kind_t {
    BARN_NODE_FUNCTION_DECLARATION,
    BARN_NODE_FUNCTION_RETURN,
    BARN_NODE_FUNCTION_CALL,

    BARN_NODE_VARIABLE_DECLARATION,

    BARN_NODE_EXPRESSION,
} barn_node_kind_t;

typedef struct __barn_expression_value_t barn_expression_value_t;

typedef struct __barn_node_t {
    barn_node_kind_t node_kind;

    struct {
        barn_array_t* function_args;
        barn_array_t* function_nodes;
        barn_type_t*  function_return;
        char*         function_name;
    } function_declaration;

    struct {
        barn_node_t* return_value;
        barn_node_t* return_func;
    } function_return;

    struct {
        barn_node_t*     variable_value;
        barn_variable_t* variable;
    } variable_declaration;

    struct {
        barn_array_t* expression_nodes;
    } expression;

    struct {
        barn_token_t* function_token;
        barn_array_t* function_args;
        const char*   function_name;
        barn_node_t*  function;
    } function_call;
} barn_node_t;

char* barn_node_kind_show(barn_node_kind_t kind);
void barn_node_show(barn_node_t* node);

barn_node_t* barn_create_empty_node(barn_node_kind_t kind);

#endif /* __BARN_NODES__ */