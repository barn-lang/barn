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

#include <barn_functions.h>
#include <barn_expressions.h>
#include <barn_array.h>
#include <barn_struct.h>

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
barn_tc_expression_check_structure_init(barn_type_checker_t* tc, barn_node_t* expr_node, 
                                        barn_array_t* expr_nodes)
{
    barn_expression_value_t* struct_expr_value = ((barn_expression_node_t*)
                                                    barn_get_element_from_array(expr_nodes, 0))->lhs;

    printf("ok\n");

    /* There is nothin to check at all */
    if (struct_expr_value->struct_values->length == 0)
        return;

    for (int i = 0; i < struct_expr_value->struct_values->length; i++)
    {
        barn_struct_field_t* field = barn_get_element_from_array(struct_expr_value->struct_type->structure.struct_fields, i);
        barn_node_t*   struct_expr = barn_get_element_from_array(struct_expr_value->struct_values, i);

        if (barn_tc_does_types_collides(barn_tc_expression_get_type(tc, struct_expr), field->field_type))
        {
            BARN_TYPE_CHECKER_ERR(tc, struct_expr_value->expr_val_token, BARN_TYPE_ERROR, 
                                  "the %s structure value is not expected type which was %s, but got %s", 
                                  barn_display_numbers_ordinal(i + 1), 
                                  barn_convert_type_to_string(field->field_type),
                                  barn_convert_type_to_string(barn_tc_expression_get_type(tc, struct_expr)));
        }
    }
}

void
barn_tc_expression_check(barn_type_checker_t* tc, barn_node_t* expr_node)
{
    barn_array_t* expr_nodes = expr_node->expression.expression_nodes;

    if (expr_nodes->length == 0)
        BARN_UNIMPLEMENTED("this shouldn't happend");

    /* If there is only one node there is not reason for 
     * getting through this whole function */
    if (expr_nodes->length == 1 && 
        ((barn_expression_node_t*)barn_get_element_from_array(expr_nodes, 0))
            ->lhs->initalizing_struct == true)
        barn_tc_expression_check_structure_init(tc, expr_node, expr_nodes);
    else if (expr_nodes->length == 1 && 
            ((barn_expression_node_t*)barn_get_element_from_array(expr_nodes, 0))
                ->lhs->is_function_call == false &&
            ((barn_expression_node_t*)barn_get_element_from_array(expr_nodes, 0))
                ->rhs == NULL)
        return;
    else if (((barn_expression_node_t*)barn_get_element_from_array(expr_nodes, 0))
                ->lhs->is_function_call == true)
    {
        barn_tc_func_call_check(tc, ((barn_expression_node_t*)barn_get_element_from_array(expr_nodes, 0))
                                    ->lhs->function_call);
        return;
    }

    for (int i = 0; i < expr_nodes->length; i++)
    {
        barn_expression_node_t* curr_expr_node = barn_get_element_from_array(expr_nodes, i);

        if (curr_expr_node->rhs != NULL)
            if (curr_expr_node->rhs->is_function_call == true)
                barn_tc_func_call_check(tc, curr_expr_node->rhs->function_call);

        if (i == 0 || (curr_expr_node->lhs != NULL && curr_expr_node->rhs != NULL))
        {
            if (curr_expr_node->rhs == NULL)
            {
                continue;
            }

            if (curr_expr_node->lhs->is_function_call == true)
                barn_tc_func_call_check(tc, curr_expr_node->lhs->function_call);
            barn_tc_does_types_collide_expr_value(curr_expr_node->lhs, curr_expr_node->rhs);
            
            if (barn_tc_does_types_collide_expr_value(curr_expr_node->lhs, curr_expr_node->rhs))
                BARN_TYPE_CHECKER_ERR(tc, curr_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s but got %s",
                    barn_convert_type_to_string(curr_expr_node->lhs->expr_val_type), 
                    barn_convert_type_to_string(curr_expr_node->rhs->expr_val_type));
        }
        else
        {
            barn_expression_node_t* last_node = barn_get_element_from_array(expr_nodes, i - 1);

            // Situation where lhs and rhs is NULL this can happend in expressions
            // like this underneath:
            //              y
            // (1 + 2) * (3 + 4)
            //    x    ^~~~
            //           LHS and RHS is being filled with parents so we can't 
            //           check them
            //
            // In this case we need to take last element of "x" expression 
            // and first of "y" expression and check do these types collides
            if (curr_expr_node->lhs == NULL && curr_expr_node->rhs == NULL)
            {
                // Now we are doing "we need to take last element of "x" expression"
                // so this is last element of "x"
                barn_expression_value_t* last_element_in_x = last_node->rhs;

                // Then we need to collect first element of "y"
                barn_expression_node_t* next_node = barn_get_element_from_array(expr_nodes, i + 1);
                barn_expression_value_t* first_element_in_y = next_node->lhs != NULL 
                                                                ? next_node->lhs : next_node->rhs;

                BARN_NO_NULL(last_element_in_x);
                BARN_NO_NULL(first_element_in_y);

                if (last_element_in_x->is_function_call == true)
                    barn_tc_func_call_check(tc, last_element_in_x->function_call);

                if (first_element_in_y->is_function_call == true)
                    barn_tc_func_call_check(tc, first_element_in_y->function_call);

                if (barn_tc_does_types_collide_expr_value(last_element_in_x, first_element_in_y))
                    BARN_TYPE_CHECKER_ERR(tc, last_element_in_x->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s but got %s",
                        barn_convert_type_to_string(last_element_in_x->expr_val_type), 
                        barn_convert_type_to_string(first_element_in_y->expr_val_type)); 

                continue;
            }

            if (last_node->rhs == NULL)
                continue;

            if (curr_expr_node->rhs != NULL)
                if (last_node->rhs->is_function_call == true)
                    barn_tc_func_call_check(tc, last_node->rhs->function_call);

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
    // A very simple function that checks does two given types don't collide
    // with each other, for example if lhs=T<string> and rhs=T<i32>
    // these two types will gonna collide because we can't make any action
    // with them.
    //
    // TODO: Add custom structure comparasion (structures first)

    if (lhs->is_any || rhs->is_any)
        return false;

    if (rhs == NULL)
        return true;

    if ((lhs->is_string && rhs->is_string) ||
        (lhs->is_char   && rhs->is_char)   ||
        (lhs->is_ptr    && rhs->is_ptr))
        return false;

    if ((lhs->is_float    && rhs->is_float)   ||
        (lhs->is_signed   && rhs->is_signed)  ||
        (lhs->is_unsigned && rhs->is_unsigned))
        return false;

    if ((lhs->is_bool && rhs->is_bool))
        return false;

    if (barn_is_type_number(lhs->type) &&
        barn_is_type_number(rhs->type))
        return false;

    if ((lhs->is_struct && rhs->is_struct) &&
        BARN_STR_CMP(lhs->structure.sturct_type_name, rhs->structure.sturct_type_name))
        return false;

    return true;
}

bool
barn_tc_does_types_collide_expr_value(barn_expression_value_t* lhs, barn_expression_value_t* rhs)
{
    return (barn_tc_does_types_collides(lhs->expr_val_type, rhs->expr_val_type));
}

void
barn_tc_func_return_check(barn_type_checker_t* tc, barn_node_t* return_node)
{
    barn_node_t* curr_node = tc->curr_node;
    if (return_node != NULL)
        curr_node = return_node;

    /* To check is function return node correct in terms
     * of types we run couple of functions */
    barn_tc_expression_check(tc, curr_node->function_return.return_value);

    barn_type_t* func_return_type = curr_node->function_return.return_func
                                        ->function_declaration.function_return;
    barn_type_t* expr_type        = barn_tc_expression_get_type(tc, curr_node->function_return.return_value);
    
    if (barn_tc_does_types_collides(func_return_type, expr_type))
    {
        barn_expression_node_t* first_expr_node = barn_get_element_from_array(curr_node->function_return.return_value
                                                                                 ->expression.expression_nodes, 0);
                                                                                 
        BARN_TYPE_CHECKER_ERR(tc, first_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s as return type got %s",
                    barn_convert_type_to_string(func_return_type), 
                    barn_convert_type_to_string(expr_type)); 
    }
}

void
barn_tc_variable_declaration(barn_type_checker_t* tc, barn_node_t* var_node)
{
    barn_node_t* curr_node = tc->curr_node;
    if (var_node != NULL)
        curr_node = var_node;

    barn_tc_expression_check(tc, curr_node->variable_declaration.variable_value);

    barn_type_t* var_expected_value_type = curr_node->variable_declaration.variable->var_type;
    barn_type_t* var_value_expr_type     = barn_tc_expression_get_type(tc, curr_node->variable_declaration.variable_value);
    
    /* This code make auto variables work, how? Uhh we just take 
     * expression type and swap it into the variable. */
    if (var_expected_value_type->type == BARN_TYPE_AUTO)
    {
        curr_node->variable_declaration.variable->var_type = barn_create_type(var_value_expr_type->type);
        return;
    }

    if (barn_tc_does_types_collides(var_expected_value_type, var_value_expr_type))
    {
        barn_expression_node_t* first_expr_node = barn_get_element_from_array(curr_node->variable_declaration.variable_value
                                                                                ->expression.expression_nodes, 0);
                                                                                 
        BARN_TYPE_CHECKER_ERR(tc, first_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, variable type is %s not %s",
                    barn_convert_type_to_string(var_expected_value_type),
                    barn_convert_type_to_string(var_value_expr_type)); 
    }
}

void
barn_tc_func_call_check(barn_type_checker_t* tc, barn_node_t* call_node)
{
    barn_node_t* curr_node = tc->curr_node;
    if (call_node != NULL)
        curr_node = call_node;
        
    barn_array_t* func_args_expected = curr_node->function_call.function
                                            ->function_declaration.function_args;
    barn_array_t* func_args_given    = curr_node->function_call.function_args;

    BARN_ARRAY_FOR(func_args_expected)
    {
        barn_func_argument_t* func_argument_expected = barn_get_element_from_array(func_args_expected, i);
        barn_node_t*          func_argument_given    = barn_get_element_from_array(func_args_given, i);

        if (func_argument_expected->argument_type->type == BARN_TYPE_FORMAT)
            break;

        barn_tc_expression_check(tc, func_argument_given);
        barn_type_t* argument_expr_type = barn_tc_expression_get_type(tc, func_argument_given);

        barn_expression_node_t* first_expr_node = barn_get_element_from_array(func_argument_given->expression.expression_nodes, 0);

        if (barn_tc_does_types_collides(argument_expr_type, func_argument_expected->argument_type))
            BARN_TYPE_CHECKER_ERR(tc, first_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s as argument type but instead got %s",
                    barn_convert_type_to_string(func_argument_expected->argument_type),
                    barn_convert_type_to_string(argument_expr_type)); 
    }
}

void
barn_tc_condition_statement(barn_type_checker_t* tc, barn_node_t* cond_node)
{
    barn_node_t* curr_node = cond_node == NULL ? tc->curr_node : cond_node;

    if (curr_node->condition_statement.kind_of_cond != BARN_ELSE_CONDITION)
        barn_tc_expression_check(tc, curr_node->condition_statement.condition_expr);
}

void
barn_tc_while_loop(barn_type_checker_t* tc, barn_node_t* cond_node)
{
    barn_node_t* curr_node = cond_node == NULL ? tc->curr_node : cond_node;

    barn_tc_expression_check(tc, curr_node->while_loop.condition_expr);
}

void
barn_tc_value_modification(barn_type_checker_t* tc, barn_node_t* value_mod)
{
    barn_node_t* curr_node = value_mod == NULL ? tc->curr_node : value_mod;
        
    if (curr_node->node_kind == BARN_NODE_VARIABLE_ASNPLUS        ||
        curr_node->node_kind == BARN_NODE_VARIABLE_ASNMINUS       ||
        curr_node->node_kind == BARN_NODE_VARIABLE_ASNDIV         ||
        curr_node->node_kind == BARN_NODE_VARIABLE_ASNMUL         || 
        curr_node->node_kind == BARN_NODE_VARIABLE_INCREMENTATION || 
        curr_node->node_kind == BARN_NODE_VARIABLE_DECREMENTATION)
    {
        if (!barn_is_type_number(curr_node->variable_modification.variable->var_type->type))
            BARN_TYPE_CHECKER_ERR(tc, curr_node->variable_modification.variable_token, BARN_TYPE_ERROR, 
                    "type mismatch, only number types or pointers can be modified like this not %s",
                    barn_convert_type_to_string(curr_node->variable_modification.variable->var_type)); 
    }

    if (curr_node->node_kind == BARN_NODE_VARIABLE_INCREMENTATION || 
        curr_node->node_kind == BARN_NODE_VARIABLE_DECREMENTATION)
        return;

    // curr_node->variable_modification.variable
    barn_tc_expression_check(tc, curr_node->variable_modification.variable_value);

    barn_type_t* new_value_type = barn_tc_expression_get_type(tc, curr_node->variable_modification.variable_value);

    if (barn_tc_does_types_collides(curr_node->variable_modification.variable->var_type,
                                    new_value_type))
    {
        barn_expression_node_t* first_expr_node = barn_get_element_from_array(curr_node->variable_modification.variable_value
                                                    ->expression.expression_nodes, 0);
        BARN_TYPE_CHECKER_ERR(tc, first_expr_node->lhs->expr_val_token, BARN_TYPE_ERROR, "type mismatch, expected %s as new variable value but instead got %s",
                    barn_convert_type_to_string(curr_node->variable_modification.variable->var_type),
                    barn_convert_type_to_string(new_value_type)); 
    }
}

void
barn_tc_enum(barn_type_checker_t* tc, barn_node_t* enum_node)
{
    barn_node_t* curr_node = enum_node == NULL ? tc->curr_node : enum_node;

    // TODO: implement enum type checking

    return;
}

void
barn_type_checker_main_loop(barn_type_checker_t* tc, barn_parser_t* parser)
{
    for (tc->index = 0; tc->index < tc->nodes->length; tc->index++)
    {
        barn_tc_advance(tc, 0);

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
            tc->index = save_index;
        }
        else if (tc->curr_node->node_kind == BARN_NODE_FUNCTION_CALL)
            barn_tc_func_call_check(tc, NULL);
        else if (tc->curr_node->node_kind == BARN_NODE_FUNCTION_RETURN)
            barn_tc_func_return_check(tc, NULL);
        else if (tc->curr_node->node_kind == BARN_NODE_EXPRESSION    ||
                 tc->curr_node->node_kind == BARN_NODE_END_STATEMENT ||
                 tc->curr_node->node_kind == BARN_NODE_BREAK_LOOP    ||
                 tc->curr_node->node_kind == BARN_NODE_CONTINUE_LOOP ||
                 tc->curr_node->node_kind == BARN_NODE_STRUCT        ||
                 tc->curr_node->node_kind == BARN_NODE_IMPORT_C)
            continue;
        else if (tc->curr_node->node_kind == BARN_NODE_VARIABLE_DECLARATION)
            barn_tc_variable_declaration(tc, NULL);
        else if (tc->curr_node->node_kind == BARN_NODE_VARIABLE_ASN            || 
                 tc->curr_node->node_kind == BARN_NODE_VARIABLE_ASNPLUS        ||
                 tc->curr_node->node_kind == BARN_NODE_VARIABLE_ASNMINUS       ||
                 tc->curr_node->node_kind == BARN_NODE_VARIABLE_ASNDIV         ||
                 tc->curr_node->node_kind == BARN_NODE_VARIABLE_ASNMUL         || 
                 tc->curr_node->node_kind == BARN_NODE_VARIABLE_INCREMENTATION || 
                 tc->curr_node->node_kind == BARN_NODE_VARIABLE_DECREMENTATION)
            barn_tc_value_modification(tc, NULL);
        else if (tc->curr_node->node_kind == BARN_NODE_CONDITION_STATEMENT)
            barn_tc_condition_statement(tc, NULL);
        else if (tc->curr_node->node_kind == BARN_NODE_WHILE_LOOP)
            barn_tc_while_loop(tc, NULL);
        else if (tc->curr_node->node_kind == BARN_NODE_ENUM)
            barn_tc_enum(tc, NULL);
        else
        {
            printf("%s\n", barn_node_kind_show(tc->curr_node->node_kind));
            BARN_UNIMPLEMENTED("unimplemenented node kind type check");
        }
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