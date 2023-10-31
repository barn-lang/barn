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
#include <barn_if.h>

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

    barn_node_t* if_node = barn_create_empty_node(BARN_NODE_IF_STATEMENT);
    if_node->if_statement.if_condition_expr = if_statement;

    barn_parser_append_node(parser, if_node);

    if (parser->curr_token->kind == BARN_TOKEN_OPENBRACE)
    {
        parser->statement_open += 1;
        parser->statement_node =  if_node;
    }
    else
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after if condition", 0);
}