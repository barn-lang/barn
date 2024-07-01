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
#include <barn_conditions.h>

#include <barn_expressions.h>
#include <barn_parser.h>
#include <barn_types.h>

void 
barn_parser_if_statement(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"if\" keyword", 0);

    barn_parser_skip(parser, 1);   
    barn_node_t* if_statement = barn_parse_expression(parser, BARN_TOKEN_OPENBRACE, BARN_TOKEN_OPENBRACE, false);

    barn_node_t* if_node = barn_create_empty_node(BARN_NODE_CONDITION_STATEMENT);
    if_node->condition_statement.condition_expr = if_statement;
    if_node->condition_statement.kind_of_cond   = BARN_IF_CONDITION;

    barn_parser_append_node(parser, if_node);

    if (parser->curr_token->kind == BARN_TOKEN_OPENBRACE)
    {
        parser->statement_open += 1;
        parser->statement_node =  if_node;

        parser->layer++;
    }
    else
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after if condition", 0);
}

void 
barn_parser_elif_statement(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"elif\" keyword", 0);

    barn_node_t* last_node = barn_get_element_from_array(parser->actual_function->function_declaration.function_nodes, 
                                                         parser->actual_function->function_declaration.function_nodes->length - 1);
    
    if (last_node == NULL)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"elif\" token after \"if\" statement expression", 0);
    
    if (last_node->node_kind != BARN_NODE_END_STATEMENT)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"elif\" token after \"if\" statement expression", 0);

    if (last_node->end_statement.kind_of_cond == BARN_IF_CONDITION || 
        last_node->end_statement.kind_of_cond == BARN_ELIF_CONDITION)
    {
        barn_parser_skip(parser, 1);
        barn_node_t* elif_statement = barn_parse_expression(parser, BARN_TOKEN_OPENBRACE, BARN_TOKEN_OPENBRACE, false);

        barn_node_t* elif_node = barn_create_empty_node(BARN_NODE_CONDITION_STATEMENT);
        elif_node->condition_statement.condition_expr  = elif_statement;
        elif_node->condition_statement.kind_of_cond   = BARN_ELIF_CONDITION;

        barn_parser_append_node(parser, elif_node);

        if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            barn_parser_skip(parser, 1);

        if (parser->curr_token->kind == BARN_TOKEN_OPENBRACE)
        {
            parser->statement_open += 1;
            parser->statement_node =  elif_node;

            parser->layer++;
        }
        else
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after elif condition", 0);
    }   
    else
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"elif\" token after \"if\" or another \"elif\" statement expression", 0);

    // if (last_node->end_statement.kind_of_cond != BARN_ELIF_CONDITION)
        // BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"elif\" token after \"if\" or another \"elif\" statement expression", 0);
}

void 
barn_parser_else_statement(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"else\" keyword", 0);
    
    barn_node_t* last_node = barn_get_element_from_array(parser->actual_function->function_declaration.function_nodes, 
                                                         parser->actual_function->function_declaration.function_nodes->length - 1);
    
    if (last_node == NULL)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"else\" token after \"if\" or \"elif\" statement expression", 0);

    if (last_node->node_kind != BARN_NODE_END_STATEMENT)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"else\" token after \"if\" or \"elif\" statement expression", 0);

    if (last_node->condition_statement.kind_of_cond == BARN_IF_CONDITION ||
        last_node->condition_statement.kind_of_cond == BARN_ELIF_CONDITION)
    {
        // if (last_node->end_statement.kind_of_cond != BARN_ELIF_CONDITION)
            // BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"else\" token after \"if\" or \"elif\" statement expression", 0);

        barn_node_t* else_node = barn_create_empty_node(BARN_NODE_CONDITION_STATEMENT);
        else_node->condition_statement.condition_expr = NULL;
        else_node->condition_statement.kind_of_cond   = BARN_ELSE_CONDITION;

        barn_parser_append_node(parser, else_node);
        barn_parser_skip(parser, 1);

        if (parser->curr_token->kind == BARN_TOKEN_OPENBRACE)
        {
            parser->statement_open += 1;
            parser->statement_node =  else_node;

            parser->layer++;
        }
        else
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after else condition", 0);
    }
    else
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"else\" token after \"if\" or \"elif\" statement expression", 0);
}