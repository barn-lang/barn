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

#include <barn_core.h>

#include <barn_expressions.h>
#include <barn_func_call.h>
#include <barn_tokens.h>
#include <barn_parser.h>

barn_expression_value_t* 
barn_create_expression_value(barn_token_t* expr_val_token,
                             barn_type_t* expr_val_type)
{
    barn_expression_value_t* expr_value = (barn_expression_value_t*)calloc(1, sizeof(barn_expression_value_t));
    BARN_NO_NULL(expr_value);
    
    expr_value->expr_val_token = expr_val_token;
    expr_value->expr_val_type  = expr_val_type;

    return expr_value;
}

barn_expression_node_t*  barn_create_expression_node(barn_expression_value_t* lhs, barn_expression_value_t* rhs,
                                                     barn_token_kind_t operator,   unsigned long parents)
{
    barn_expression_node_t* expr_node = (barn_expression_node_t*)calloc(1, sizeof(barn_expression_node_t));
    BARN_NO_NULL(expr_node);

    expr_node->lhs = lhs;
    expr_node->rhs = rhs;

    expr_node->operator = operator;
    expr_node->parents  = parents;

    return expr_node;
}

barn_node_t*  
barn_create_expression_ast_node()
{
    barn_node_t* node = barn_create_empty_node(BARN_NODE_EXPRESSION);
    node->expression.expression_nodes = barn_create_array(sizeof(barn_expression_node_t*));

    return node;
}

barn_expression_value_t* 
barn_get_expr_value(barn_parser_t* parser, barn_expr_parser_t* expr_parser)
{
    barn_expression_value_t* expr_value = barn_create_expression_value(NULL, barn_get_type_i32_global());

    // TODO: implement:
    //          - [X] variables
    //          - [X] setting expr_val_type by checking the token value length
    //          - [ ] casting ("(type)value")

    if (parser->curr_token->kind == BARN_TOKEN_INT)
    {
        expr_value->expr_val_token = parser->curr_token;
        expr_value->expr_val_type  = barn_get_type_i32_global();
        printf("%s\n", parser->curr_token->value);
    } 
    else if (parser->curr_token->kind == BARN_TOKEN_FLOAT)
    {
        expr_value->expr_val_token = parser->curr_token;
        expr_value->expr_val_type  = barn_get_type_f32_global();
    }
    else if (parser->curr_token->kind == BARN_TOKEN_STRING)
    {
        expr_value->expr_val_token = parser->curr_token;
        expr_value->expr_val_type = barn_get_type_str_global();
    }
    else if (parser->curr_token->kind == BARN_TOKEN_CHAR)
    {
        expr_value->expr_val_token = parser->curr_token;
        expr_value->expr_val_type = barn_create_type(BARN_TYPE_I8);
        
        expr_value->expr_val_type->is_char = true;
    }
    else if (parser->curr_token->kind == BARN_TOKEN_EOF)
    {
        // barn_parser_skip(parser, -1);
        parser->index += -1;
        parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected value token not EOF", 
                        parser->curr_token->value);
    }
    else if (parser->curr_token->kind == BARN_TOKEN_OPENPARENT)
    {
        // barn_parser_skip(parser, 1);
        parser->index += 1;
        parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);
        return NULL;
    }
    else if (parser->curr_token->kind == BARN_TOKEN_IDENTIFIER)
    {
        if (barn_parser_is_next_token(parser, BARN_TOKEN_OPENPARENT))
        {
            barn_node_t* func_call = __barn_parser_func_call(parser);

            expr_value->expr_val_type = func_call->function_call.function->function_declaration.function_return;
            expr_value->expr_val_token = func_call->function_call.function_token;
            expr_value->function_call = func_call;
            expr_value->is_function_call = true;
        }
        else if (barn_parser_is_variable_defined_lg(parser, parser->curr_token->value))
        {
            barn_variable_t* variable = barn_parser_get_variable_by_name(parser, parser->curr_token->value);

            expr_value->expr_val_token   = parser->curr_token;
            expr_value->expr_val_type    = variable->var_type;
            expr_value->is_function_call = false;
            expr_value->is_variable      = true;
        }
        else if (BARN_STR_CMP(parser->curr_token->value, "true")  || 
                 BARN_STR_CMP(parser->curr_token->value, "false"))
        {
            expr_value->expr_val_token   = parser->curr_token;
            expr_value->expr_val_type    = barn_get_type_bool_global();
            expr_value->is_function_call = false;
            expr_value->is_variable      = true; 
        }
        else
            BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "given identifier \"%s\" is undefined", 
                            parser->curr_token->value);
    }
    else 
    {
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of \"%s\" in this place", 
                        parser->curr_token->value);
    }

    // barn_parser_skip(parser, 1);
    parser->index += 1;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

    return expr_value;
}

bool
barn_expression_is_curr_operator(barn_parser_t* parser)
{
    switch (parser->curr_token->kind)
    {
        case BARN_TOKEN_PLUS: // normal operators
        case BARN_TOKEN_MINUS:
        case BARN_TOKEN_MUL:
        case BARN_TOKEN_OPENPARENT:
        case BARN_TOKEN_CLOSEPARENT:
        case BARN_TOKEN_DIV:
            return true;
            break;
        case BARN_TOKEN_GT: // condiction operators
        case BARN_TOKEN_GTE:
        case BARN_TOKEN_LT:
        case BARN_TOKEN_LTE:
        case BARN_TOKEN_EQ:
        case BARN_TOKEN_NEQ:
            return true;
            break;
        default:
            return false;
            break;
    }

    return false;
}

int
barn_expression_parser_check_parents(barn_parser_t* parser, barn_expr_parser_t* expr_parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_OPENPARENT)
    {
        if (expr_parser->index == 0)
            expr_parser->index = -1;

        printf("parents ++; %d\n", __LINE__);
        expr_parser->parents++;
        parser->index += 1;
        parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

        return 1;
    }
    
    if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT)
    { 
        if (expr_parser->parents == 0)
        {
            if (expr_parser->function_argument_value == true)
                return 2;
            else
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of close parent", 0);
        }

        expr_parser->parents--; 
        printf("closing one parent %d\n", expr_parser->parents);
        // barn_parser_skip(parser, 1);
        parser->index += 1;
        parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

        return 1;
    }

    return 0;
}

void
barn_expression_parser_check_rhs_value_null(barn_parser_t* parser, barn_expression_value_t* rhs, 
                                            barn_expr_parser_t* expr_parser)
{
    if (rhs == NULL)
    {
        expr_parser->index = -1;
        // barn_parser_skip(parser, -1);
        parser->index -= 1;
        parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);
    }
}

void
barn_expression_parser_not_full_op_rhs(barn_parser_t* parser, barn_expr_parser_t* expr_parser)
{
    // Collect operator
    barn_token_kind_t operator = parser->curr_token->kind;
    if (!barn_expression_is_curr_operator(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected operator not \"%s\"", 
                        barn_token_kind_to_string(parser->curr_token->kind));
    // barn_parser_skip(parser, 1);
    parser->index += 1;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);


    if (operator == BARN_TOKEN_DIV)
    {
        barn_expression_value_t* lhs_value = barn_get_element_from_array(expr_parser->main_expr_node->expression.expression_nodes, 
                                                                         expr_parser->main_expr_node->expression.expression_nodes->length - 1);
        printf("%p\n", lhs_value->expr_val_token);
        barn_expression_division_by_zero(parser, expr_parser, lhs_value);
    }

    // Get rhs value
    if (parser->curr_token->kind == BARN_TOKEN_EOF) 
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected expression value not EOF", 0);
    barn_expression_value_t* rhs_value = barn_get_expr_value(parser, expr_parser);

    if (operator == BARN_TOKEN_DIV)
        barn_expression_division_by_zero(parser, expr_parser, rhs_value);

    barn_expression_parser_check_rhs_value_null(parser, rhs_value, expr_parser);

    // Append new mathematical expression node
    barn_expression_node_t* append_expr_node = barn_create_expression_node(NULL, rhs_value, operator, expr_parser->parents);
    barn_append_element_to_array(expr_parser->main_expr_node->expression.expression_nodes, append_expr_node);

    if (rhs_value == NULL)
    {
        printf("parents ++; %d\n", __LINE__);
        // expr_parser->parents++;
    }
}

void
barn_expression_division_by_zero(barn_parser_t* parser, barn_expr_parser_t* expr_parser, 
                                 barn_expression_value_t* expr_value)
{
    if (expr_value->expr_val_token == NULL)
        return;

    if (expr_value->expr_val_token->kind == BARN_TOKEN_INT)
    {
        /* Convert string value of int token into 
         * int64_t AKA long long */
        int64_t converted_value = atoll(expr_value->expr_val_token->value);

        if (converted_value != 0)
            return;
    }
    else if (expr_value->expr_val_token->kind == BARN_TOKEN_FLOAT)
    {
        /* Convert string value of float token into
         * double long */
        double converted_value = atof(expr_value->expr_val_token->value);

        if (converted_value != 0)
            return;
    }
    else
        return;

    // barn_parser_skip(parser, -1);
    parser->index += -1;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

    BARN_PARSER_ERR(parser, BARN_MATH_ERROR, "division by 0 is not allowed", 0);
}

void
barn_expression_parser_full_lhs_op_rhs(barn_parser_t* parser,  barn_expr_parser_t* expr_parser)
{
    // Get lhs value
    barn_expression_value_t* lhs_value = barn_get_expr_value(parser, expr_parser);

    if (parser->curr_token->kind == BARN_TOKEN_EOF          || 
        parser->curr_token->kind == BARN_TOKEN_NEWLINE      ||
        parser->curr_token->kind == expr_parser->end_kind   ||
        parser->curr_token->kind == expr_parser->end_kind_2)
    {
        barn_expression_node_t* append_expr_node = barn_create_expression_node(lhs_value, NULL, BARN_TOKEN_NONE, expr_parser->parents);
        barn_append_element_to_array(expr_parser->main_expr_node->expression.expression_nodes, append_expr_node);
        return;
    }

    // Collect operator
    barn_token_kind_t operator = parser->curr_token->kind;
    if (!barn_expression_is_curr_operator(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected operator not \"%s\"", 
                        barn_token_kind_to_string(parser->curr_token->kind));
    // barn_parser_skip(parser, 1);
    parser->index += 1;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

    if (operator == BARN_TOKEN_DIV)
        barn_expression_division_by_zero(parser, expr_parser, lhs_value);

    // Get rhs value
    if (parser->curr_token->kind == BARN_TOKEN_EOF) 
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected expression value not EOF", 0);
    barn_expression_value_t* rhs_value = barn_get_expr_value(parser, expr_parser);

    if (operator == BARN_TOKEN_DIV)
        barn_expression_division_by_zero(parser, expr_parser, rhs_value);

    barn_expression_parser_check_rhs_value_null(parser, rhs_value, expr_parser);

    // Append new mathematical expression node
    barn_expression_node_t* append_expr_node = barn_create_expression_node(lhs_value, rhs_value, operator, expr_parser->parents);
    barn_append_element_to_array(expr_parser->main_expr_node->expression.expression_nodes, append_expr_node);

    if (rhs_value == NULL || lhs_value == NULL)
    {
        expr_parser->parents++;
        printf("parents ++; %d\n", __LINE__);
    }
}

barn_expr_parser_t*
barn_create_expr_parser(bool function_argument_value, barn_node_t* expr_node,
                        barn_token_kind_t end_kind, barn_token_kind_t end_kind_2)
{
    barn_expr_parser_t* expr_parser = (barn_expr_parser_t*)calloc(1, sizeof(barn_expr_parser_t));
    expr_parser->function_argument_value = function_argument_value;
    expr_parser->main_expr_node          = expr_node;
    expr_parser->end_kind_2              = end_kind_2;
    expr_parser->end_kind                = end_kind;
    expr_parser->parents                 = 0;
    expr_parser->index                   = 0;

    return expr_parser;
}

barn_node_t* 
barn_parse_expression(barn_parser_t* parser, barn_token_kind_t end_kind, 
                      barn_token_kind_t end_kind_2, bool function_argument_value)
{
    barn_node_t* expr_node = barn_create_expression_ast_node();

    if (parser->curr_token->kind == end_kind || parser->curr_token->kind == end_kind_2)
        return expr_node;
    
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected expression value not EOF", 0);

    barn_expr_parser_t* expr_parser = barn_create_expr_parser(function_argument_value, expr_node,
                                                              end_kind, end_kind_2);

    for (; ; expr_parser->index++)
    {
        printf("%s\n", parser->curr_token->value);

        int parents_ret = barn_expression_parser_check_parents(parser, expr_parser);

        if (parents_ret == 1)
            continue;
        else if (parents_ret == 2)
            break;

        if (parser->curr_token->kind == BARN_TOKEN_EOF     || 
            parser->curr_token->kind == BARN_TOKEN_NEWLINE ||
            parser->curr_token->kind == end_kind_2         ||
            parser->curr_token->kind == end_kind)
            break;

        if (expr_parser->index == 0)
            barn_expression_parser_full_lhs_op_rhs(parser, expr_parser);
        else 
            barn_expression_parser_not_full_op_rhs(parser, expr_parser);
    }

    for (int i = 0; i < expr_node->expression.expression_nodes->length; i++)
    {
        barn_expression_node_t* curr_expr_node = barn_get_element_from_array(expr_node->expression.expression_nodes, i);
       
        // printf("lhs: \"%s\", rhs: \"%s\", op: \"%s\", parents: %d\n",
        //     curr_expr_node->lhs != NULL
        //         ? (curr_expr_node->lhs->expr_val_token->value)
        //         : "(null)",
        //     curr_expr_node->rhs != NULL
        //         ? curr_expr_node->rhs->expr_val_token->value
        //         : "(null)",
        //     barn_token_kind_to_string(curr_expr_node->operator),
        //     curr_expr_node->parents);
    }

    // if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
    // {
    //     parser->index += 1;
    //     parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);
    // }

    return expr_node;
}