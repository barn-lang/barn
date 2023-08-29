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

#define BARN_TOKEN_CMP(str) (strcmp(parser->curr_token->value, str) == 0)

/* Function for skipping tokens */
void 
barn_parser_skip(barn_parser_t* parser, int n)
{
    parser->index += n;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);
}

void
barn_parser_function_declaration(barn_parser_t* parser)
{
    
}

void
barn_parser_identifier(barn_parser_t* parser)
{
    if (BARN_TOKEN_CMP("fun"))
        barn_parser_function_declaration(parser);
}

void 
barn_parser_main_loop(barn_parser_t* parser)
{
    for (; parser->index < parser->lexer->tokens->length; parser->index++)
    {
        barn_parser_skip(parser, 0);
        // printf("%s\n", parser->curr_token->value);

        if (parser->curr_token->kind == BARN_TOKEN_EOF)
            break;
        else if (parser->curr_token->kind == BARN_TOKEN_IDENTIFIER)
            barn_parser_identifier(parser);
        else 
        {
            barn_error_show_with_line(parser->lexer,
                BARN_SYNTAX_ERROR, parser->curr_token->filename,
                parser->curr_token->row - 1, parser->curr_token->col - 1, 
                true, parser->curr_token->line, "unknown use of '%s' token in this place", 
                parser->curr_token->value);
            exit(1);
        }
    }
}

barn_parser_t* 
barn_start_parser(barn_lexer_t* lexer)
{
    barn_parser_t* parser = (barn_parser_t*)calloc(1, sizeof(barn_parser_t));
    
    parser->global_variables = barn_create_array(sizeof(barn_node_t));
    parser->structure_nodes  = barn_create_array(sizeof(barn_node_t));
    parser->local_variables  = barn_create_array(sizeof(barn_node_t));
    parser->function_nodes   = barn_create_array(sizeof(barn_node_t));
    parser->nodes            = barn_create_array(sizeof(barn_node_t));

    parser->curr_token = NULL;
    parser->lexer      = lexer;

    parser->statement_open = 0;
    parser->index          = 0;

    parser->actual_function = NULL;

    barn_debug_entry("barn_parser_main_loop", __FILE__, __LINE__);
    barn_parser_main_loop(parser);
    return parser;
}

bool 
barn_parser_is_function_opened(barn_parser_t* parser)
{
    return (parser->actual_function == NULL)
        ? false 
        : true;
}