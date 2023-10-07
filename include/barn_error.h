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

#ifndef __BARN_ERROR__
#define __BARN_ERROR__

#include <barn_core.h>

#define BARN_ERROR_CODES_LENGTH 11

typedef enum __barn_error_types_t {
	BARN_ARGUMENT_ERROR,
	BARN_FILE_ERROR,
	BARN_SYNTAX_ERROR,
	BARN_PARSER_ERROR,
	BARN_UNDEFINED_ERROR,
	BARN_TYPE_ERROR,
	BARN_UNKNOWN_ERROR,
	BARN_COMPILER_ERROR,
	BARN_OVERFLOW_ERROR,
	BARN_NAMESPACE_ERROR,
	BARN_MATH_ERROR,
} barn_error_types_t;

typedef struct __barn_lexer_t barn_lexer_t;

const char* barn_error_code_to_string(barn_error_types_t error_type);

void barn_error_show_with_line(barn_lexer_t* lexer, barn_error_types_t error_type, char* filename, 
                          	   int row, int col, bool is_line, char* line, char* fmt, ...);

void barn_error_show(barn_error_types_t error_type, char* message, ...);

#endif /* __BARN_ERROR__ */