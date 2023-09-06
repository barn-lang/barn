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

#include <barn_colors.h>
#include <barn_debug.h>
#include <barn_error.h>
#include <barn_lexer.h>

const char* barn_error_codes_string[BARN_ERROR_CODES_LENGTH] = {
    [BARN_ARGUMENT_ERROR ] = "ArgumentError",
	[BARN_FILE_ERROR     ] = "FileError",
	[BARN_SYNTAX_ERROR   ] = "SyntaxError",
	[BARN_PARSER_ERROR   ] = "ParserError",
	[BARN_UNDEFINED_ERROR] = "UndefinedError",
	[BARN_UNKNOWN_ERROR  ] = "UnknownError",
	[BARN_COMPILER_ERROR ] = "CompilerError",
	[BARN_OVERFLOW_ERROR ] = "OverflowError",
	[BARN_NAMESPACE_ERROR] = "NamespaceError",
};

const char* 
barn_error_code_to_string(barn_error_types_t error_type)
{
    if (error_type >= BARN_ERROR_CODES_LENGTH)
        return barn_error_codes_string[BARN_UNDEFINED_ERROR];

    return barn_error_codes_string[(int)error_type];
}

void
barn_error_show_with_line(barn_lexer_t* lexer, barn_error_types_t error_type, char* filename, 
                          int row, int col, bool is_line, char* line, char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);

    char buf[512];
    vsnprintf(buf, 512, fmt, list);

    printf("%s%s%s: %s:%d:%d: %s\n",
		barn_get_bold_color_as_str_code(BARN_COLOR_RED),
		barn_error_code_to_string(error_type),
		barn_get_color_as_str_code(BARN_COLOR_RESET),
		filename, row, col, buf);

	if (is_line == false && lexer == NULL)
        return;

    if (row != 0) 
        printf("%s%d %s| %s...%s\n", 
            barn_get_color_as_str_code(BARN_COLOR_GREEN), row-1, barn_get_color_as_str_code(BARN_COLOR_GRAY), 
            barn_get_color_as_str_code(BARN_COLOR_GRAY), barn_get_color_as_str_code(BARN_COLOR_RESET));
    
    printf("%s%d %s| %s", 
            barn_get_color_as_str_code(BARN_COLOR_GREEN), row, 
            barn_get_color_as_str_code(BARN_COLOR_GRAY), barn_get_color_as_str_code(BARN_COLOR_GRAY));
    for (int i = 0; i < strlen(line); i++)
    {
        if (i == col)
            printf("%s", barn_get_color_as_str_code(BARN_COLOR_GREEN));

        if (line[i] == ' ' || line[i] == '(' || line[i] == '|' || line[i] == ')' || line[i] == '{' || 
            line[i] == '}' || line[i] == '[' || line[i] == ']' || line[i] == ']' || line[i] == ':' ||
            line[i] == '"' || line[i] == '\'')
            printf("%s", barn_get_color_as_str_code(BARN_COLOR_GRAY));

        printf("%c", line[i]);
    }

    memset(buf, 512, 0);
    sprintf(buf, "%d |", row);

    printf("\n");
    for (int i = 0; i < strlen(buf); i++)
        printf(" ");

    for (int i = 0; i < col + 1; i++)
        printf(" ");
    printf("%s^---%s error occurred here\n", barn_get_color_as_str_code(BARN_COLOR_GREEN), barn_get_color_as_str_code(BARN_COLOR_WHITE));

    // if (lexer->file_lines->length <= (row + 1))
    // {
    //     printf("%s\n", barn_get_color_as_str_code(BARN_COLOR_GRAY));
    //     printf("%s%d %s| %s...%s\n\n", 
    //         barn_get_color_as_str_code(BARN_COLOR_GREEN), row+1, 
    //         barn_get_color_as_str_code(BARN_COLOR_GRAY), 
    //         barn_get_color_as_str_code(BARN_COLOR_GRAY), 
    //         barn_get_color_as_str_code(BARN_COLOR_RESET));
    // }
    // else
    // {
    //     printf("%s\n", barn_get_color_as_str_code(BARN_COLOR_GRAY));
    //     printf("%s%d %s| %s%s%s\n\n", 
    //         barn_get_color_as_str_code(BARN_COLOR_GREEN), row+1, 
    //         barn_get_color_as_str_code(BARN_COLOR_GRAY), 
    //         barn_get_color_as_str_code(BARN_COLOR_GRAY), 
    //         barn_get_element_from_array(lexer->file_lines, row+1),
    //         barn_get_color_as_str_code(BARN_COLOR_RESET));
    // }

    va_end(list);
}

void
barn_error_show(barn_error_types_t error_type, char* message, ...)
{
    va_list list;
    va_start(list, message);

    char buf[512];
    vsnprintf(buf, 512, message, list);

    printf("%s%s%s: %s\n",
		barn_get_bold_color_as_str_code(BARN_COLOR_RED),
		barn_error_code_to_string(error_type),
		barn_get_color_as_str_code(BARN_COLOR_RESET),
		buf);
    
    va_end(list);
}