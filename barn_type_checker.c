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

#include <barn_type_checker.h>
#include <barn_core.h>

#include <barn_parser.h>
#include <barn_tokens.h>
#include <barn_lexer.h>
#include <barn_nodes.h>

#include <barn_expressions.h>
#include <barn_array.h>

#ifndef BARN_TYPE_CHECKER_ERR
# define BARN_TYPE_CHECKER_ERR(tc, token, error_type, msg, ...) ({   \
     barn_error_show_with_line(tc->parser->lexer,                    \
         error_type, token->filename,                                \
         token->row - 1, token->col - 1,                             \
         true, token->line, msg, __VA_ARGS__);                       \
     exit(1);                                                        \
 })
#endif /* BARN_TYPE_CHECKER_ERR */

void
barn_tc_advance(barn_type_checker_t* tc, int count)
{
    tc->index     += count;   
    tc->curr_node  = barn_get_element_from_array(tc->nodes, tc->index);
}

void
barn_tc_expression_check(barn_type_checker_t* tc, barn_node_t* expr_node)
{
    barn_array_t* expr_nodes = expr_node->expression.expression_nodes;

    for (int i = 0; i < expr_nodes->length; i++)
    {
        barn_expression_node_t* curr_expr_node = barn_get_element_from_array(expr_nodes, i);

        if (i == 0)
        {
            if (barn_tc_does_types_collide_expr_value(curr_expr_node->lhs, curr_expr_node->rhs))
                BARN_TYPE_CHECKER_ERR(tc, curr_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s but got %s",
                    barn_convert_type_to_string(curr_expr_node->lhs->expr_val_type), 
                    barn_convert_type_to_string(curr_expr_node->rhs->expr_val_type));
        }
        else
        {
            barn_expression_node_t* last_node = barn_get_element_from_array(expr_nodes, i - 1);

            if (barn_tc_does_types_collide_expr_value(last_node->rhs, curr_expr_node->rhs))
                BARN_TYPE_CHECKER_ERR(tc, last_node->rhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s but got %s",
                    barn_convert_type_to_string(last_node->rhs->expr_val_type), 
                    barn_convert_type_to_string(curr_expr_node->rhs->expr_val_type)); 
        }
    }
}

barn_type_t*
barn_tc_expression_get_type(barn_type_checker_t* tc, barn_node_t* expr_node)
{
    barn_expression_node_t* last_expr_node 
        = barn_get_element_from_array(expr_node->expression.expression_nodes, 0);

    return last_expr_node->lhs->expr_val_type;
}

bool
barn_tc_does_types_collides(barn_type_t* lhs, barn_type_t* rhs)
{
    // A very simple function that checks does two given type don't collide
    // with each other, for example if lhs=typeof(string) and rhs=typeof(i32)
    // these two types will gonna collide because we can't make any action
    // with them.
    //
    // TODO: Add custom structure comparasion (structures first)

    if (rhs == NULL)
        return false;

    if ((lhs->is_string && rhs->is_string) ||
        (lhs->is_ptr    && rhs->is_ptr))
        return false;

    if ((lhs->is_float    && rhs->is_float)   ||
        (lhs->is_signed   && rhs->is_signed)  ||
        (lhs->is_unsigned && rhs->is_unsigned))
        return false;

    if (barn_is_type_number(lhs->type) &&
        barn_is_type_number(rhs->type))
        return false;
    
    return true;
}

bool
barn_tc_does_types_collide_expr_value(barn_expression_value_t* lhs, barn_expression_value_t* rhs)
{
    return (barn_tc_does_types_collides(lhs->expr_val_type, rhs->expr_val_type));
}

void
barn_tc_func_return_check(barn_type_checker_t* tc)
{
    /* To check is function return node correct in terms
     * of types we run couple of functions */
    barn_tc_expression_check(tc, tc->curr_node->function_return.return_value);

    barn_type_t* func_return_type = tc->curr_node->function_return.return_func
                                        ->function_declaration.function_return;
    barn_type_t* expr_type        = barn_tc_expression_get_type(tc, tc->curr_node->function_return.return_value);
    
    if (barn_tc_does_types_collides(func_return_type, expr_type))
    {
        barn_expression_node_t* first_expr_node = barn_get_element_from_array(tc->curr_node->function_return.return_value
                                                                                 ->expression.expression_nodes, 0);
                                                                                 
        BARN_TYPE_CHECKER_ERR(tc, first_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s as return type got %s",
                    barn_convert_type_to_string(func_return_type), 
                    barn_convert_type_to_string(expr_type)); 
    }
}

void
barn_tc_func_call_check(barn_type_checker_t* tc)
{
    
}

void
barn_type_checker_main_loop(barn_type_checker_t* tc, barn_parser_t* parser)
{
    for (tc->index = 0; tc->index < tc->nodes->length; tc->index++)
    {
        barn_tc_advance(tc, 0);
        printf("%s\n", barn_node_kind_show(tc->curr_node->node_kind));

        if (tc->curr_node->node_kind == BARN_NODE_FUNCTION_DECLARATION)
        {
            // Make a copy of index so we can use it later
            int save_index = tc->index;

            // Set values for new type checker
            tc->nodes = tc->curr_node->function_declaration.function_nodes;
            tc->index = 0;
            barn_tc_advance(tc, 0);

            // Enter type checker loop and give old nodes to it
            barn_type_checker_main_loop(tc, parser);
            tc->nodes = parser->nodes;
        }
        else if (tc->curr_node->node_kind == BARN_NODE_FUNCTION_CALL)
            barn_tc_func_call_check(tc);
        else if (tc->curr_node->node_kind == BARN_NODE_FUNCTION_RETURN)
            barn_tc_func_return_check(tc);
    }
}

void
barn_type_checker_start(barn_parser_t* parser)
{
    barn_type_checker_t* tc = (barn_type_checker_t*)calloc(1, sizeof(barn_type_checker_t));
    tc->curr_node = NULL;
    tc->nodes     = parser->nodes;
    tc->index     = 0;
    tc->parser    = parser;

    barn_type_checker_main_loop(tc, parser);
}