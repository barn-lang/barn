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
#include <barn_std.h>
#include <barn_io.h>

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

        if (barn_file_exists(filename_with_std) == true)
            file_value = barn_read_whole_file(filename_with_std);
        else
        {
            barn_error_show_with_line(include_struct->main_lexer,
                BARN_PARSER_ERROR, (char*)include_struct->curr_token->filename, include_struct->curr_token->row, 
                include_struct->curr_token->col - 1, true, include_struct->curr_token->line,
                "couldn't find file named \'%s\'", filename);
            exit(1); 
        }

        filename = filename_with_std;
    }

    
    /* Create a custom args parser in which we would apply filename of imported file
     * and then after it start lexer work */
    barn_args_parser_t* custom_args_parser = (barn_args_parser_t*)calloc(1, sizeof(barn_args_parser_t));
    custom_args_parser->is_filename   = true;
    custom_args_parser->filename      = filename;

    barn_debug_entry("barn_start_lexer", __FILE__, __LINE__);
    barn_lexer_t* file_lexer = barn_start_lexer((const char*)file_value, custom_args_parser);
    
    barn_delete_element_from_array(include_struct->main_lexer->tokens, include_struct->index);
    barn_delete_element_from_array(include_struct->main_lexer->tokens, include_struct->index - 1);

    int add_tokens_at = include_struct->index - 1;

    for (int i = 0; i < (file_lexer->tokens->length - 1); i++)
    {
        barn_token_t* token = barn_get_element_from_array(file_lexer->tokens, i);
        barn_insert_element_in_array(include_struct->main_lexer->tokens, i + add_tokens_at, token);
    }
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