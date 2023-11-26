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
#include <barn_expressions.h>

void 
barn_parser_func_return(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"return\" inside function body", 0);

    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected return expression value not EOF", 0);  

    /* Parse expression that will be present after
     * return keyword 
     * 
     * return <expression>
     * 
     * After parsing this expression we create a node
     * assign return value into it and append to node
     * array
     * */
    barn_node_t* return_value     = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_NONE, false);
    barn_node_t* func_return_node = barn_create_empty_node(BARN_NODE_FUNCTION_RETURN);

    func_return_node->function_return.return_value = return_value;
    func_return_node->function_return.return_func  = parser->actual_function;
    barn_parser_append_node(parser, func_return_node);
}