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

#include <barn_core.h>

#include <barn_expressions.h>
#include <barn_func_call.h>
#include <barn_types.h>
#include <barn_tokens.h>
#include <barn_struct.h>
#include <barn_parser.h>
#include <barn_type_checker.h>

barn_expression_value_t* 
barn_create_expression_value(barn_token_t* expr_val_token,
                             barn_type_t* expr_val_type)
{
    barn_expression_value_t* expr_value = (barn_expression_value_t*)calloc(1, sizeof(barn_expression_value_t));
    BARN_NO_NULL(expr_value);
    
    expr_value->expr_val_token   = expr_val_token;
    expr_value->expr_val_type    = expr_val_type;
    expr_value->fields_of_struct = barn_create_array(sizeof(barn_struct_field_t*));

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

void
barn_parser_expression_access_structure_fields(barn_parser_t* parser, barn_expr_parser_t* expr_parser, 
                                               barn_expression_value_t* expr_value)
{
    barn_variable_t* parent_variable = barn_parser_get_variable_by_name(parser, parser->curr_token->value);
    barn_parser_skip(parser, 1);

    if (!barn_is_type_struct(parent_variable->var_type->type))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of \".\" because \"%s\" is not a structure type", 
                        parent_variable->var_name);
    
    expr_value->is_variable = true;
    barn_append_element_to_array(expr_value->fields_of_struct, parent_variable->var_name);

    barn_type_t* current_type = parent_variable->var_type;

    while (parser->curr_token->kind == BARN_TOKEN_DOT)
    {
        if (parser->curr_token->kind == BARN_TOKEN_DOT)
            barn_parser_skip(parser, 1);

        const char* field_name = parser->curr_token->value;

        if (!barn_is_type_struct(current_type->type))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "couldn't find field named \"%s\" in \"%s\" type because it's not a structure", 
                            field_name, barn_convert_type_to_string(current_type));

        if (!barn_parser_is_structure_field_named(parser, current_type, field_name))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "couldn't find field named \"%s\" in \"%s\" type", 
                            field_name, current_type->structure.sturct_type_name);

        barn_append_element_to_array(expr_value->fields_of_struct, parser->curr_token->value);
        barn_struct_field_t* field = barn_parser_get_structure_field_named(parser, current_type, field_name);
        current_type = field->field_type;

        barn_parser_skip(parser, 1);
    }

    expr_value->accessing_struct = true;
    expr_value->expr_val_token   = barn_get_element_from_array(parser->lexer->tokens, parser->index);
    expr_value->expr_val_type    = current_type;
    expr_value->is_variable      = true;

    parent_variable->is_used = true;
    barn_parser_skip(parser, -1);

    // fuck this code, how could i write this whole 
    // compiler but struggle with implementing structure fields
    // brooo covid is eating my mind really 
    //  ~ Solindek 08.12.23 13:19

    // nvm it works i'm just dumb lol 
    //  ~ Solindek 08.12.23 13:26
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
            barn_node_t* func_call = __barn_parser_func_call(parser, true);

            expr_parser->main_expr_node->expression.is_compiler_time = false;

            expr_value->expr_val_type = func_call->function_call.function->function_declaration.function_return;
            expr_value->expr_val_token = func_call->function_call.function_token;
            expr_value->function_call = func_call;
            expr_value->is_function_call = true;
        }
        else if (barn_parser_is_variable_defined_lg(parser, parser->curr_token->value))
        {
            if (barn_parser_is_next_token(parser, BARN_TOKEN_DOT))
                barn_parser_expression_access_structure_fields(parser, expr_parser, expr_value);
            else
            {
                barn_variable_t* variable = barn_parser_get_variable_by_name(parser, parser->curr_token->value);

                expr_value->expr_val_token   = parser->curr_token;
                expr_value->expr_val_type    = variable->var_type;
                expr_value->is_function_call = false;
                expr_value->is_variable      = true;

                variable->is_used = true;
            }
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
        case BARN_TOKEN_MOD:
        case BARN_TOKEN_OPENPARENT:
        case BARN_TOKEN_CLOSEPARENT:
        case BARN_TOKEN_DIV:
            return true;
            break;
        case BARN_TOKEN_GT: // condition operators
        case BARN_TOKEN_GTE:
        case BARN_TOKEN_LT:
        case BARN_TOKEN_LTE:
        case BARN_TOKEN_EQ:
        case BARN_TOKEN_NEQ:
        case BARN_TOKEN_ANDAND:
        case BARN_TOKEN_OROR:
            return true;
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
        expr_parser->parents++;
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

barn_array_t* 
barn_parse_expression_structure_values(barn_parser_t* parser, barn_node_t* expr_node,
                                       barn_type_t* structure_type)
{
    barn_array_t* structure_values = barn_create_array(sizeof(barn_node_t*));

    while (true)
    {
        if (parser->curr_token->kind == BARN_TOKEN_EOF)
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected value or \",\"", 0);
        if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT)
            return structure_values;

        barn_node_t* argument_value = barn_parse_expression(parser, BARN_TOKEN_COMMA, BARN_TOKEN_CLOSEBRACE, false);
        barn_append_element_to_array(structure_values, argument_value);

        if (barn_tc_expression_get_type(NULL, argument_value)->is_struct)
            barn_parser_skip(parser, 1);

        if (parser->curr_token->kind == BARN_TOKEN_COMMA)
        {
            barn_parser_skip(parser, 1);
            continue;
        }
        else if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
            return structure_values;
        else
        {
            if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            {
                barn_parser_skip(parser, -1);
                if (parser->curr_token->kind == BARN_TOKEN_COMMA)
                {
                    barn_parser_skip(parser, 1);
                    continue;
                }
                else if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
                    return structure_values;
                else
                    BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \",\" or \"}\" after structure value", 0);
            }
            else
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \",\" or \"}\" after structure value", 0);
        }
    }

    return structure_values;
}

void
barn_parse_expression_new_structure(barn_parser_t* parser, barn_node_t* expr_node, barn_expr_parser_t* expr_parser)
{
    /* Compiler time set to false */
    expr_node->expression.is_compiler_time = false;
    barn_token_t* start_token = parser->curr_token;

    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind != BARN_TOKEN_IDENTIFIER)
        BARN_PARSER_ERR(parser, BARN_TYPE_ERROR, "expected identifier that represents a struct type", 0);

    barn_type_t* structure_type = barn_parser_current_token_type_representation(parser);
    if (!barn_is_type_struct(structure_type->type))
        BARN_PARSER_ERR(parser, BARN_TYPE_ERROR, "type named \"%s\" is not an structure", parser->curr_token->value);
    
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_OPENBRACE))
        BARN_PARSER_ERR(parser, BARN_TYPE_ERROR, "expected \"{\" to create a new structure", 0);
    barn_parser_skip(parser, 1);

    barn_array_t* structure_values = barn_create_array(sizeof(barn_node_t*));
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_CLOSEBRACE))
    {
        barn_parser_skip(parser, 1);
        structure_values = barn_parse_expression_structure_values(parser, expr_node, structure_type);

        if (structure_values->length != structure_type->structure.struct_fields->length)
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "there is too much or too less structure value %s", structure_type->structure.sturct_type_name);
    }
    else
        barn_parser_skip(parser, 1);

    // for (int i = 0; i < structure_values->length; i++)
    // {
    //     barn_node_t* curr_args_node = barn_get_element_from_array(structure_values, i);
    //
    //     printf("structure_values[%d]={ expression_nodes: %p, expression_nodes->length: %d }\n", i,
    //         curr_args_node->expression.expression_nodes,
    //         curr_args_node->expression.expression_nodes->length);
    // }

    /* Pack up all of it into a expression node */
    barn_expression_value_t* structure_value = barn_create_expression_value(start_token, structure_type);
    structure_value->initalizing_struct      = true;
    structure_value->struct_type             = structure_type;
    structure_value->struct_values           = structure_values;

    barn_expression_node_t* struct_expr_node = barn_create_expression_node(structure_value, NULL, BARN_TOKEN_NONE, 0);
    barn_append_element_to_array(expr_parser->main_expr_node->expression.expression_nodes, struct_expr_node);
}

barn_node_t* 
barn_parse_expression(barn_parser_t* parser, barn_token_kind_t end_kind, 
                      barn_token_kind_t end_kind_2, bool function_argument_value)
{
    barn_node_t* expr_node = barn_create_expression_ast_node();
    expr_node->expression.is_compiler_time = true;

    if (parser->curr_token->kind == end_kind || parser->curr_token->kind == end_kind_2)
        return expr_node;
    
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected expression value not EOF", 0);

    barn_expr_parser_t* expr_parser = barn_create_expr_parser(function_argument_value, expr_node,
                                                              end_kind, end_kind_2);

    /* Handle creating structures */
    if (parser->curr_token->kind == BARN_TOKEN_IDENTIFIER &&
        BARN_STR_CMP(parser->curr_token->value, "new"))
    {
        barn_parse_expression_new_structure(parser, expr_node, expr_parser);
        return expr_node;
    }

    for (; ; expr_parser->index++)
    {
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