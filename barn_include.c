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

#include <barn_include.h>
#include <barn_tokens.h>
#include <barn_array.h>
#include <barn_lexer.h>
#include <barn_io.h>

const char*
barn_std_get_path() { return "example/path/to/std/"; }

void
barn_include_tokenize_file_and_append_tokens(barn_include_t* include_struct)
{
    char* filename = include_struct->curr_token->value;
    char* file_value = NULL;

    if (barn_file_exists(filename) == true)
        file_value = barn_read_whole_file(filename);

    if (file_value == NULL)
    {
        const char* std_path = barn_std_get_path();

        char* filename_with_std = calloc(strlen(std_path) + strlen(filename) + 2, sizeof(char));
        sprintf(filename_with_std, "%s%s", std_path, filename);

        /* TODO: check does this file exists, read it */
    }

    /* TODO: tokenize file_value and append these tokens to main_lexer somehow.. */
}

void
barn_include_check_is_import(barn_include_t* include_struct)
{
    /* Given token isn't "@import" keyword */
    if (strcmp(include_struct->curr_token->value, "@import") != 0)
        return;

    /* Get another token which is import file name */
    include_struct->index++;
    include_struct->curr_token = barn_get_element_from_array(include_struct->main_lexer->tokens, include_struct->index);

    if (include_struct->curr_token->kind != BARN_TOKEN_STRING)
    {
        barn_error_show_with_line(include_struct->main_lexer,
                BARN_PARSER_ERROR, (char*)include_struct->curr_token->filename, include_struct->curr_token->row, 
                include_struct->curr_token->col - 1, true, include_struct->curr_token->line,
                "expected import filename after \'@import\' keyword");
        exit(1);
    }

    barn_include_tokenize_file_and_append_tokens(include_struct);
}

void
barn_include_files(barn_lexer_t* lexer)
{
    barn_include_t* include_struct = (barn_include_t*)calloc(1, sizeof(barn_include_t));

    include_struct->main_lexer = lexer;
    include_struct->curr_token = NULL;
    include_struct->index      = 0;

    for (include_struct->index = 0; include_struct->index < lexer->tokens->length; include_struct->index++)
    {
        include_struct->curr_token = barn_get_element_from_array(lexer->tokens, include_struct->index);

        if (include_struct->curr_token->kind == BARN_TOKEN_IDENTIFIER)
            barn_include_check_is_import(include_struct);
    }

    free(include_struct);
}