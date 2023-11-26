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
#include <barn_parser.h>

void 
barn_parser_while_loop(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"while\" keyword", 0);

    barn_parser_skip(parser, 1);
    barn_node_t* while_statement = barn_parse_expression(parser, BARN_TOKEN_OPENBRACE, BARN_TOKEN_OPENBRACE, false);

    barn_node_t* while_node = barn_create_empty_node(BARN_NODE_WHILE_LOOP);
    while_node->while_loop.condition_expr = while_statement;

    barn_parser_append_node(parser, while_node);

    if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
        barn_parser_skip(parser, 1);

    if (parser->curr_token->kind == BARN_TOKEN_OPENBRACE)
    {
        parser->loop_open     += 1;
        parser->statement_open += 1;
        parser->statement_node =  while_node;
    }
    else
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after while condition", 0);
}

void 
barn_parser_for_loop(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"for\" keyword", 0);

    BARN_UNIMPLEMENTED("for loops are not implemented right now");
}

void
barn_parser_break_loop(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"while\" keyword", 0);

    if (!(parser->loop_open >= 1))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "break keyword can be used only inside for or while loops", 0);

    barn_node_t* break_node = barn_create_empty_node(BARN_NODE_BREAK_LOOP);
    barn_parser_append_node(parser, break_node);
}

void
barn_parser_continue_loop(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to use \"while\" keyword", 0);

    if (!(parser->loop_open >= 1))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "continue keyword can be used only inside for or while loops", 0);

    barn_node_t* continue_node = barn_create_empty_node(BARN_NODE_CONTINUE_LOOP);
    barn_parser_append_node(parser, continue_node);
}