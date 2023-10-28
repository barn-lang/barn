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

#ifndef __BARN_EXPRESSIONS__
#define __BARN_EXPRESSIONS__

#include <barn_core.h>

#include <barn_tokens.h>
#include <barn_parser.h>

typedef enum __barn_expression_value_type_t {
    BARN_EXPRESSION_VALUE_FUNCTION_CALL,
    BARN_EXPRESSION_VALUE_VARIABLE,
    BARN_EXPRESSION_VALUE,
} barn_expression_value_type_t;

typedef struct __barn_expression_value_t {
    barn_token_t* expr_val_token;
    barn_type_t*  expr_val_type;

    bool is_variable;
    
    bool is_function_call;
    barn_node_t* function_call;
} barn_expression_value_t;

typedef struct __barn_expression_node_t {
    barn_expression_value_t* lhs;
    barn_expression_value_t* rhs;
    
    barn_token_kind_t operator;
    unsigned long     parents;
} barn_expression_node_t;

typedef struct __barn_expr_parser_t {
    long index;
    long parents;

    barn_token_kind_t end_kind;
    barn_token_kind_t end_kind_2;

    bool function_argument_value;

    barn_node_t* main_expr_node;
} barn_expr_parser_t;

barn_expression_value_t* barn_create_expression_value(barn_token_t* expr_val_token, barn_type_t* expr_val_type);
barn_expression_node_t*  barn_create_expression_node(barn_expression_value_t* lhs, barn_expression_value_t* rhs,
                                                     barn_token_kind_t operator,   unsigned long parents);
barn_node_t*             barn_create_expression_ast_node();

barn_node_t* barn_parse_expression(barn_parser_t* parser, barn_token_kind_t end_kind, 
                                   barn_token_kind_t end_kind_2, bool function_argument_value);

void barn_expression_division_by_zero(barn_parser_t* parser, barn_expr_parser_t* expr_parser, 
                                      barn_expression_value_t* expr_value);

#endif /* __BARN_EXPRESSIONS__ */