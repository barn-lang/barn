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
#include <barn_functions.h>
#include <barn_expressions.h>
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

#define BARN_KEYWORDS_LEN 15

static const char* barn_const_keywords[BARN_KEYWORDS_LEN] = {
    [0 ] = "fun",
    [1 ] = "extern",
    [2 ] = "@import_c",
    [3 ] = "@import",
    [4 ] = "let",
    [5 ] = "const",
    [6 ] = "return",
    [7 ] = "if",
    [8 ] = "else",
    [9 ] = "elif",
    [10] = "while",
    [11] = "continue",
    [12] = "break",
    [13] = "for",
    [14] = "struct",
};

/* Function for skipping tokens */
void 
barn_parser_skip(barn_parser_t* parser, int n)
{
    parser->index += n;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);
}

bool
barn_parser_is_next_token(barn_parser_t* parser, barn_token_kind_t kind)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expeceted '%s' not EOF", 
                        barn_token_kind_to_string(parser->curr_token->kind));

    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind == kind)
    {
        barn_parser_skip(parser, -1);
        return true;
    }

    barn_parser_skip(parser, -1);
    return false;
}

bool
barn_parser_is_id_keyword(char* id_keyword)
{
    for (int i = 0; i < BARN_KEYWORDS_LEN; i++)
        if (BARN_STR_CMP(barn_const_keywords[i], id_keyword))
            return true;

    return false;
}

bool
barn_parser_is_id_correct_namespace(char* id_namespace)
{
    if (barn_parser_is_id_keyword(id_namespace) == true)
        return false;

    // TODO: check is namespace already taken by a constant variable

    return true;
}

void
barn_parser_identifier(barn_parser_t* parser)
{
    if (!barn_parser_is_id_keyword(parser->curr_token->value))
        BARN_UNIMPLEMENTED("can't parse not keyword identifier expression for now");

    if (BARN_TOKEN_CMP("fun"))
        barn_parser_function_declaration(parser);
}

void 
barn_parser_reset_local_variables(barn_parser_t* parser)
{
    barn_destroy_array(parser->local_variables);
    parser->local_variables = barn_create_array(sizeof(barn_node_t));
}

void 
barn_parser_close_brace(barn_parser_t* parser)
{
    parser->actual_function = NULL;
    barn_parser_reset_local_variables(parser);
}

void 
barn_parser_main_loop(barn_parser_t* parser)
{
    // for (; parser->index < parser->lexer->tokens->length; parser->index++)
    // {
    //     barn_parser_skip(parser, 0);

    //     if (parser->curr_token->kind == BARN_TOKEN_EOF)
    //         break;
    //     else if (parser->curr_token->kind == BARN_TOKEN_IDENTIFIER)
    //         barn_parser_identifier(parser);
    //     else if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
    //         barn_parser_close_brace(parser);
    //     else 
    //         BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unknown use of '%s' token in this place", 
    //                         parser->curr_token->value);
    // }
    barn_parser_skip(parser, 0);
    barn_parse_expression(parser, BARN_TOKEN_SEMICOL);
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

void 
barn_parser_append_node(barn_parser_t* parser, barn_node_t* node)
{
    if (barn_parser_is_function_opened(parser))
        BARN_UNIMPLEMENTED("implement appending nodes when functions are opened");

    barn_append_element_to_array(parser->nodes, node);
}