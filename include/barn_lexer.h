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

#ifndef __BARN_LEXER__
#define __BARN_LEXER__

#include <barn_core.h>
#include <barn_string.h>
#include <barn_args_parser.h>
#include <barn_array.h>
#include <barn_tokens.h>

typedef struct __barn_lexer_t {
    const char*  filename;
    const char*  file_content;

    barn_args_parser_t* args_parser;

    barn_array_t* tokens;
    barn_array_t* file_lines;

    barn_token_t* last_token;

    char curr_char;
    char next_char;

    int index;
    int col;
    int row;

    bool is_char;
    bool is_space;
    bool is_string;
    bool is_comment_inline;
    bool is_comment_multiline;
} barn_lexer_t;

typedef struct __barn_lexer_symbol_t {
    barn_token_kind_t symbol_kind;
    char*             symbol_value;
} barn_lexer_symbol_t;

barn_lexer_t* barn_start_lexer(const char* file_content, barn_args_parser_t* args_parser);

void barn_lexer_store_file_lines(barn_lexer_t* lexer, const char* filename);

void barn_lexer_main(barn_lexer_t* lexer);

void barn_lexer_show_all_tokens(barn_lexer_t* lexer);

barn_lexer_symbol_t barn_lexer_create_symbol(barn_lexer_t* lexer); 

#endif /* __BARN_LEXER__ */