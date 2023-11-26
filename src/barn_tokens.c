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

#include <barn_tokens.h>
#include <barn_string.h>

const char* 
barn_token_kind_to_string(barn_token_kind_t kind)
{
    switch (kind) {
        case BARN_TOKEN_NEWLINE:
            return "NEWLINE";
        case BARN_TOKEN_NONE:
            return "NONE";
        case BARN_TOKEN_INCREMENTATION:
            return "INCREMENTATION";
        case BARN_TOKEN_DECREMENTATION:
            return "DECREMENTATION";
        case BARN_TOKEN_EOF:
            return "EOF";
        case BARN_TOKEN_INT:
            return "INT";
        case BARN_TOKEN_CHAR:
            return "CHAR";
        case BARN_TOKEN_FLOAT:
            return "FLOAT";
        case BARN_TOKEN_STRING:
            return "STRING";
        case BARN_TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case BARN_TOKEN_PLUS:
            return "PLUS";
        case BARN_TOKEN_MINUS:
            return "MINUS";
        case BARN_TOKEN_MUL:
            return "MUL";
        case BARN_TOKEN_DIV:
            return "DIV";
        case BARN_TOKEN_PLUSASN:
            return "PLUSASN";
        case BARN_TOKEN_MINUSASN:
            return "MINUSASN";
        case BARN_TOKEN_MULASN:
            return "MULASN";
        case BARN_TOKEN_DIVASN:
            return "DIVASN";
        case BARN_TOKEN_BANG:
            return "BANG";
        case BARN_TOKEN_GT:
            return "GT";
        case BARN_TOKEN_GTE:
            return "GTE";
        case BARN_TOKEN_LT:
            return "LT";
        case BARN_TOKEN_LTE:
            return "LTE";
        case BARN_TOKEN_ASN:
            return "ASN";
        case BARN_TOKEN_EQ:
            return "EQ";
        case BARN_TOKEN_NEQ:
            return "NEQ";
        case BARN_TOKEN_OROR:
            return "OROR";
        case BARN_TOKEN_ANDAND:
            return "ANDAND";
        case BARN_TOKEN_OPENPARENT:
            return "OPENPARENT";
        case BARN_TOKEN_CLOSEPARENT:
            return "CLOSEPARENT";
        case BARN_TOKEN_OPENBRACKET:
            return "OPENBRACKET";
        case BARN_TOKEN_CLOSEBRACKET:
            return "CLOSEBRACKET";
        case BARN_TOKEN_OPENBRACE:
            return "OPENBRACE";
        case BARN_TOKEN_CLOSEBRACE:
            return "CLOSEBRACE";
        case BARN_TOKEN_COMMA:
            return "COMMA";
        case BARN_TOKEN_COLON:
            return "COLON";
        case BARN_TOKEN_DOT:
            return "DOT";
        case BARN_TOKEN_SEMICOL:
            return "SEMICOL";
        case BARN_TOKEN_MOD:
            return "MOD";
        case BARN_TOKEN_ARROW:
            return "ARROW";
        case BARN_TOKEN_TRIPLEDOT:
            return "TRIPLEDOT";
	}

	return "NONE";
}

barn_token_t* 
barn_create_token(char* value, char* filename, char* line, 
                  int col, int row, barn_token_kind_t kind)
{
    barn_token_t* token = (barn_token_t*)calloc(1, sizeof(barn_token_t));

    token->value    = barn_duplicate_string(value);
    token->filename = filename;
    token->line     = line;
    token->col      = col;
    token->row      = row;
    token->kind     = kind;

    return token;
}
                                
void 
barn_token_print(barn_token_t* token)
{
    printf(" { value: `%s`, row: %d, col: %d, kind: `%s`, filename: `%s`, line: %p }\n",
        token->value, 
        token->row,
        token->col, 
        barn_token_kind_to_string(token->kind),
        token->filename,
        token->line);
}