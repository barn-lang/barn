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
#include <barn_message.h>
#include <barn_colors.h>
#include <barn_array.h>
#include <barn_error.h>
#include <barn_debug.h>
#include <barn_main.h>
#include <barn_io.h>

void 
barn_no_filename_action(char** argv, barn_args_parser_t* args_parser)
{
    if (args_parser->flags->length == 0)
        barn_no_flags_message(argv);

    for (int i = 0; i < args_parser->flags->length; i++) 
    {
        const char* flag = barn_get_element_from_array(args_parser->flags, i);

        if (strcmp(flag, "--help") == 0 || strcmp(flag, "-h") == 0)
            barn_help_message();
        else if (strcmp(flag, "--version") == 0 || strcmp(flag, "-v") == 0)
            barn_version_message();
        else
        {
            barn_error_show(BARN_ARGUMENT_ERROR, "unknown use of '%s' flag", flag);
            exit(1);
        }
    }

    exit(0);
}

void
barn_filename_action(barn_args_parser_t* args_parser)
{
    if (barn_file_exists(args_parser->filename) == false)
    {
        barn_error_show(BARN_FILE_ERROR, "file named '%s' doesn't exists",
            args_parser->filename);
        exit(1);
    }
}

int
main(int argc, char** argv)
{
    barn_args_parser_t* args_parser = barn_args_parser_start(argc, argv);

    if (args_parser->is_filename == false)
        barn_no_filename_action(argv, args_parser);

    barn_filename_action(args_parser);

    return EXIT_SUCCESS;
}