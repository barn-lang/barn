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
#include <barn_string.h>
#include <barn_lexer.h>
#include <barn_nodes.h>
#include <barn_array.h>
#include <barn_io.h>

#ifndef BARN_TOKEN_CMP
# define BARN_TOKEN_CMP(str) (strcmp(parser->curr_token->value, str) == 0)
#endif /* BARN_TOKEN_CMP */

#ifndef BARN_PARSER_ERR
# define BARN_PARSER_ERR(parser, error_type, msg, ...) ({            \
     barn_error_show_with_line(parser->lexer,                        \
         error_type, parser->curr_token->filename,                   \
         parser->curr_token->row - 1, parser->curr_token->col - 1,   \
         true, parser->curr_token->line, msg, __VA_ARGS__);          \
     exit(1);                                                        \
 })
#endif /* BARN_PARSER_ERR */

barn_node_t*
barn_parser_function_get_by_name(barn_parser_t* parser, char* function_name)
{
    for (int i = 0; i < parser->function_nodes->length; i++)
    {
        barn_node_t* node = barn_get_element_from_array(parser->function_nodes, i);
        
        if (node->node_kind != NODE_FUNCTION_DECLARATION)
            BARN_UNIMPLEMENTED("unknown node_kind that shouldn't be here");

        if (BARN_STR_CMP(node->function_declaration.function_name, function_name))
            return node;
    }

    return NULL;
}

/* 
 * This function named `barn_parser_collect_function_name`
 * is a very interesting one because it first of all checks
 * is the current token EOF kind next we duplicate this string
 * and checks is it a correct namespace and does a function
 * like this doesn't exists already 
 */
const char*
barn_parser_collect_function_name(barn_parser_t* parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        return NULL;

    const char* function_name = barn_duplicate_string(parser->curr_token->value);
    if (!barn_parser_is_id_correct_namespace((char*)function_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "function couldn't be named \'%s\'", parser->curr_token->value);

    if (barn_parser_function_get_by_name(parser, (char*)function_name) != NULL)
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "function with this name already exists", 0);

    // TODO: does function name equal to variable name

    return function_name;
}

void
barn_parser_function_declaration(barn_parser_t* parser)
{
    if (barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function is already opened", 0);

    if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected function name after 'func' keyword", 0);
    barn_parser_skip(parser, 1);

    const char* function_name = barn_parser_collect_function_name(parser);

    if (!barn_parser_is_next_token(parser, BARN_TOEKN_OPENPARENT))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected '(' after function name", 0);
    
    barn_array_t* function_args = barn_parser_function_args_parse(parser);
}