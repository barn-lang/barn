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
#include <barn_tokens.h>
#include <barn_parser.h>

barn_expression_value_t* 
barn_create_expression_value(barn_token_t* expr_val_token,
                             barn_type_t* expr_val_type)
{
    barn_expression_value_t* expr_value = (barn_expression_value_t*)calloc(1, sizeof(barn_expression_value_t));
    expr_value->expr_val_token = expr_val_token;
    expr_value->expr_val_type  = expr_val_type;

    return expr_value;
}

barn_expression_node_t*  
barn_create_expression_node(void* lhs, bool lhs_is_node,
                           void* rhs, bool rhs_is_node,
                           barn_expression_type_t node_type)
{
    barn_expression_node_t* expr_node = (barn_expression_node_t*)calloc(1, sizeof(barn_expression_node_t));
    expr_node->lhs_ptr = lhs;
    expr_node->rhs_ptr = rhs;

    expr_node->lhs_is_node = lhs_is_node;
    expr_node->rhs_is_node = rhs_is_node;
    expr_node->node_type   = node_type;

    return expr_node;
}

barn_expression_t*       
barn_create_expression(barn_expression_node_t* node)
{
    barn_expression_t* expr = (barn_expression_t*)calloc(1, sizeof(barn_expression_t));
    expr->root_node = node;

    return expr;
}

barn_expression_value_t* 
barn_expr_get_value(barn_parser_t* parser)
{
    barn_expression_value_t* expr_value = barn_create_expression_value(NULL, NULL);

    // TODO: implement:
    //          - [ ] variables
    //          - [ ] setting expr_val_type by checking the token value length
    //          - [ ] casting ("(type)value")

    if (parser->curr_token->kind == BARN_TOKEN_INT)
    {
        expr_value->expr_val_token = parser->curr_token;
        expr_value->expr_val_type  = barn_type_i32_global;
    } 
    else if (parser->curr_token->kind == BARN_TOKEN_FLOAT)
    {
        expr_value->expr_val_token = parser->curr_token;
        expr_value->expr_val_type  = barn_type_f32_global;
    }
    else if (parser->curr_token->kind == BARN_TOKEN_EOF)
    {
        barn_parser_skip(parser, -1);
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected value token not EOF", 
                        parser->curr_token->value);
    }
    else 
    {
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of \"%s\" in this place", 
                        parser->curr_token->value);
    }

    barn_parser_skip(parser, 1);
    return expr_value;
}

barn_expression_type_t
barn_expr_detect_sign(barn_parser_t* parser)
{
    switch (parser->curr_token->kind) 
    {
        case BARN_TOKEN_PLUS:
            return BARN_EXPRESSION_ADD;
            break;
        case BARN_TOKEN_MINUS:
            return BARN_EXPRESSION_MINUS;
            break;
        case BARN_TOKEN_MUL:
            return BARN_EXPRESSION_MULTIPLY;
            break;
        case BARN_TOKEN_DIV:
            return BARN_EXPRESSION_DIVIDE;
            break;
        default:
        {
            if (parser->curr_token->kind == BARN_TOKEN_EOF)
            {
                barn_parser_skip(parser, -1);
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected expression operator not EOF", 
                                parser->curr_token->value);
            }
            else
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of \"%s\" in this place", 
                                parser->curr_token->value);
        }
    }
}

char*
barn_expr_type_to_string(barn_expression_type_t expr_type)
{
    switch (expr_type) 
    {
        case BARN_EXPRESSION_ADD:
            return "+";
            break;
        case BARN_EXPRESSION_MINUS:
            return "-";
            break;
        case BARN_EXPRESSION_MULTIPLY:
            return "*";
            break;
        case BARN_EXPRESSION_DIVIDE:
            return "/";
            break;
    }
}

barn_expression_t* 
barn_expr_parse_end_at(barn_parser_t* parser, barn_token_kind_t end_kind)
{
    barn_expression_value_t* lhs_value = barn_expr_get_value(parser);
    barn_expression_node_t*  expr_node = barn_create_expression_node(lhs_value, false, NULL, false, BARN_EXPRESSION_ADD);
   
    while (parser->curr_token->kind != BARN_TOKEN_EOF || parser->curr_token->kind != end_kind)
    {
        if (parser->curr_token->kind == BARN_TOKEN_EOF || parser->curr_token->kind == end_kind)
            break;

        printf("%s\n", parser->curr_token->value);
        barn_expression_type_t sign = barn_expr_detect_sign(parser);
        expr_node->node_type = sign;
        barn_parser_skip(parser, 1);

        barn_expression_value_t* rhs_value = barn_expr_get_value(parser);
        expr_node->rhs_is_node = false;
        expr_node->rhs_ptr     = rhs_value;
    }

    printf("%s %s %s\n", 
        ((barn_expression_value_t*)expr_node->lhs_ptr)->expr_val_token->value,
        barn_expr_type_to_string(expr_node->node_type),
        ((barn_expression_value_t*)expr_node->rhs_ptr)->expr_val_token->value);
}