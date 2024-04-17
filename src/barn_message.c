/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2024
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

#include <barn_message.h>
#include <barn_colors.h>
#include <barn_io.h>

void
barn_version_message()
{
    printf("barnc compiler %s"BARN_VERSION_FULL"%s (%s%s%s)\n",
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET),
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_get_current_architecture(),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
    
    exit(0);
}

void 
barn_no_flags_message(char** argv)
{   
    printf("See more in %s%s --help%s\n",
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        argv[0], barn_get_bold_color_as_str_code(BARN_COLOR_RESET));

    barn_version_message();
    exit(1);
}

void 
barn_help_message()
{
    // printf("%sBarn Help:%s\n", barn_get_bold_color_as_str_code(BARN_COLOR_GREEN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("  - %sFlags%s\n", barn_get_bold_color_as_str_code(BARN_COLOR_YELLOW), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--help%s show this message\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--version%s show version of barn compiler\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--c%s compile and write output of program in C (default)\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--tokens%s show all tokens from lexer (for debug purposes)\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--time%s show lexer, parser and codegen process time\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--cflags%s argument that can add c flags to compilation\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--no-main%s don't show error about no main function\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--no-stdlib%s don't include barn_header.h\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
	// printf("    > %s--no-delete-cout%s don't delete generated source code\n", barn_get_bold_color_as_str_code(BARN_COLOR_CYAN), barn_get_bold_color_as_str_code(BARN_COLOR_RESET));

    printf("barnc compiler %s"BARN_VERSION_FULL"%s (%s%s%s)\n\
%susage%s: barn <options> <filename>\n\n\
%soptions%s:\n\
  - --no-stdlib, -ns               Standard libraries are not included\n\
  - --tokens, -t                   Show all tokens generated in lexer\n\
  - --cflags                       Add flags to C compiler\n\
  - --version, -v                  Show Current version of barn compiler\n\
  - --no-delete-cout, -nc          Don't delete generated source code\n\
  - --w-disable-unused, -wd-unused Warning about unused variables are not showed\n\
  - --no-main, -nm                 Compiled file is allowed to not have a main function in it\n\
  - --time, -t                     Prints compilation time\n\
  - --help, -h                     Prints this message\n\
  - --ast, -a                      Shows Abstract Syntax Tree generated in Parser\n\
",

        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET),
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_get_current_architecture(),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET),
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET),
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET));

    exit(0);
}
