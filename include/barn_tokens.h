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

#ifndef __BARN_TOKENS__
#define __BARN_TOKENS__

#include <barn_core.h>

typedef enum __barn_token_kind_t {
    /* Special tokens */
	BARN_TOKEN_NONE = 0,
	BARN_TOKEN_EOF,

	/* Default tokens */
	BARN_TOKEN_INT,
	BARN_TOKEN_CHAR,
	BARN_TOKEN_FLOAT,
	BARN_TOKEN_STRING,
	BARN_TOKEN_IDENTIFIER,

	/* Operators */
	BARN_TOKEN_PLUS,           // "+"
	BARN_TOKEN_MINUS,          // "-"
	BARN_TOKEN_MUL,            // "*"
	BARN_TOKEN_DIV,            // "/"
	BARN_TOKEN_MOD,            // "%"
	BARN_TOKEN_BANG,           // "!"
	BARN_TOKEN_PLUSASN,        // "+="
	BARN_TOKEN_MINUSASN,       // "-="
	BARN_TOKEN_MULASN,         // "*="
	BARN_TOKEN_DIVASN,         // "/="
	BARN_TOKEN_INCREMENTATION, // "++"
	BARN_TOKEN_DECREMENTATION, // "--"

	/* Logical Operators */
	BARN_TOKEN_GT,  // ">"
	BARN_TOKEN_GTE, // ">="
	BARN_TOKEN_LT,  // "<"
	BARN_TOKEN_LTE, // "<="
	BARN_TOKEN_ASN, // "="
	BARN_TOKEN_EQ,  // "=="
	BARN_TOKEN_NEQ, // "!="

	/* Btiwise operators */
	BARN_TOKEN_OROR,   // "||"
	BARN_TOKEN_ANDAND, // "&&"

	/* Structural tokens */
	BARN_TOKEN_OPENPARENT,     // "("
	BARN_TOKEN_CLOSEPARENT,    // ")"
	BARN_TOKEN_OPENBRACKET,    // "["
	BARN_TOKEN_CLOSEBRACKET,   // "]"
	BARN_TOKEN_OPENBRACE,      // "{"
	BARN_TOKEN_CLOSEBRACE,     // "}"

	/* Seperators */
	BARN_TOKEN_COMMA,   // "," 
	BARN_TOKEN_COLON,   // ":"
	BARN_TOKEN_DOT,     // "."
	BARN_TOKEN_SEMICOL, // ";"

	/* Arrows */
	BARN_TOKEN_ARROW, // "->"
	
	/* Other */
	BARN_TOKEN_TRIPLEDOT,  // "..."
} barn_token_kind_t;

typedef struct __barn_token_t {
    char* value;
    char* filename;
    char* line;

    int col;
    int row;

    barn_token_kind_t kind;
} barn_token_t;

const char* barn_token_kind_to_string(barn_token_kind_t kind);

barn_token_t* barn_create_token(char* value, char* filename, char* line, 
                                int col, int row, barn_token_kind_t kind);
                                
void barn_token_print(barn_token_t* token);

#endif /* __BARN_TOKENS__ */