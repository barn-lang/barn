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

#include <barn_args_parser.h>
#include <barn_string.h>
#include <barn_array.h>
#include <barn_error.h>

barn_args_parser_t* 
barn_args_parser_start(int argc, char** argv)
{
    barn_args_parser_t* args_parser = (barn_args_parser_t*)calloc(1, sizeof(barn_args_parser_t));

    args_parser->argc = argc;
    args_parser->argv = argv;

    args_parser->is_filename = false;

    args_parser->flags = barn_create_array(sizeof(char*));

    for (int i = 1; i < args_parser->argc; i++)
    {
        char* arg = args_parser->argv[i];

        if (arg[0] == '-')
        {
            BARN_TRY(barn_append_element_to_array(args_parser->flags, arg));
        }
        else 
        {
            if (args_parser->is_filename == false)
            {
                args_parser->is_filename = true;
                args_parser->filename = barn_duplicate_string(arg);
            }
            else 
            {
                barn_error_show(BARN_ARGUMENT_ERROR, "unknown use of argument '%s'", arg);
                exit(EXIT_FAILURE);
            }
        }
    }

    return args_parser;
}

char* 
barn_get_flag_by_index(barn_args_parser_t* args_parser, int index)
{
    if (index < 0) 
        return NULL;

    if (index > args_parser->flags->length)
        return NULL;

    return (char*)barn_get_element_from_array(args_parser->flags, index);
}

int 
barn_get_flag_index(barn_args_parser_t* args_parser, char* flag)
{
    for (int i = 0; i < args_parser->flags->length; i++)
        if (barn_get_element_from_array(args_parser->flags, i) == flag)
            return i;

    return -1;
}

bool 
barn_is_flag(barn_args_parser_t* args_parser, char* flag)
{
    int index = barn_get_flag_index(args_parser, flag);

    return index == -1 
                ? true 
                : false;
}