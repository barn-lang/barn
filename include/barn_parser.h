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

#ifndef __BARN_PARSER__
#define __BARN_PARSER__

#include <barn_core.h>

#include <barn_types.h>
#include <barn_array.h>
#include <barn_string.h>
#include <barn_nodes.h>
#include <barn_lexer.h>
#include <barn_io.h>

typedef struct __barn_parser_t {
    barn_array_t* nodes;
    barn_lexer_t* lexer;
    barn_token_t* curr_token;
    
    int statement_open;
    int index;

    barn_array_t* function_nodes;
    barn_array_t* local_variables;
    barn_array_t* global_variables;
    barn_array_t* structure_nodes;

    barn_node_t* statement_node;
    barn_node_t* actual_function;
} barn_parser_t;

#define BARN_TOKEN_CMP(str) (strcmp(parser->curr_token->value, str) == 0)

#define BARN_PARSER_ERR(parser, error_type, msg, ...) ({            \
    barn_error_show_with_line(parser->lexer,                        \
        error_type, parser->curr_token->filename,                   \
        parser->curr_token->row - 1, parser->curr_token->col - 1,   \
        true, parser->curr_token->line, msg, __VA_ARGS__);          \
    exit(1);                                                        \
})

void barn_parser_skip(barn_parser_t* parser, int n);

barn_parser_t* barn_start_parser(barn_lexer_t* lexer);

void barn_parser_main_loop(barn_parser_t* parser);

bool barn_parser_is_function_opened(barn_parser_t* parser);
bool barn_parser_is_next_token(barn_parser_t* parser, barn_token_kind_t kind);
bool barn_parser_is_id_keyword(char* id_keyword);
bool barn_parser_is_id_correct_namespace(char* id_namespace);

#endif /* __BARN_PARSER__ */