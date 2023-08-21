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

#include <barn_lexer.h>
#include <barn_array.h>
#include <barn_string.h>

barn_lexer_t* 
barn_start_lexer(const char* file_content, barn_args_parser_t* args_parser)
{
    barn_lexer_t* lexer = (barn_lexer_t*)malloc(sizeof(barn_lexer_t));

    lexer->file_content = file_content;
    lexer->filename     = args_parser->filename;

    lexer->args_parser = args_parser;
    lexer->tokens      = barn_create_array(sizeof(barn_token_t*));
    lexer->file_lines  = barn_create_array(sizeof(barn_token_t*));
    lexer->last_token  = NULL;

    lexer->curr_char = '\0';
    lexer->next_char = '\0';
    lexer->index     = 0;

    lexer->is_char               = 0;
    lexer->is_space              = 0;
    lexer->is_string             = 0;
    lexer->is_comment_inline     = 0;
    lexer->is_comment_multiline  = 0;

    lexer->row = 0;
    lexer->col = 0;

    barn_lexer_store_file_lines(lexer);
    barn_lexer_main(lexer);
    barn_lexer_show_all_tokens(lexer);

    return lexer;
}

void 
barn_lexer_store_file_lines(barn_lexer_t* lexer)
{
    size_t length = strlen(lexer->file_content);

    if (length == 0)
        return;

    /* Crete first line */
    barn_append_element_to_array(lexer->file_lines, barn_create_allocated_string());

    for (int i = 0; i < length; i++)
    {
        char curr_char = lexer->file_content[i];

        if (curr_char == '\n' && i != length - 1)
            barn_append_element_to_array(lexer->file_lines, barn_create_allocated_string());
        else
        {
            if (i == length - 1)
                return;

            char* last_line = barn_get_element_from_array(lexer->file_lines, lexer->file_lines->length - 1);
            barn_append_char_to_allocated_string(last_line, curr_char);
        }
    }
}

char 
barn_lexer_get_next_char(barn_lexer_t* lexer)
{
    if ((strlen(lexer->file_content) - 1) == lexer->index)
        return 0;

    return lexer->file_content[lexer->index + 1];
}

barn_token_t* 
barn_lexer_get_last_token(barn_lexer_t* lexer)
{
    if (lexer->tokens->length == 0)
        return NULL;

    return barn_get_element_from_array(lexer->tokens, lexer->tokens->length - 1); 
}

void 
barn_lexer_advance(barn_lexer_t* lexer, int count)
{
    lexer->index += count;

    lexer->curr_char  = lexer->file_content[lexer->index];
    lexer->next_char  = barn_lexer_get_next_char(lexer);
    lexer->last_token = barn_lexer_get_last_token(lexer);

    lexer->col++;
}

void 
barn_lexer_new_line(barn_lexer_t* lexer)
{
    lexer->row ++;
    lexer->col = 0;
    lexer->is_space = true;
    lexer->is_comment_inline = false; 
}

void 
barn_lexer_space(barn_lexer_t* lexer)
{
    if (lexer->is_string == true)
    {
        char* last_tok_value = lexer->last_token->value;
        barn_append_char_to_allocated_string(last_tok_value, ' ');
    }
    
    lexer->is_space = true;
}

void 
barn_lexer_comment_inline(barn_lexer_t* lexer)
{
    if (lexer->is_string == true)
    {
        char* last_tok_value = lexer->last_token->value;
        barn_append_char_to_allocated_string(last_tok_value, '/');
    }
    else
    {
        lexer->is_comment_inline = true;
        lexer->is_space = true;
    }
}

void 
barn_lexer_comment_multiline_open(barn_lexer_t* lexer)
{
    if (lexer->is_string == true)
    {
        char* last_tok_value = lexer->last_token->value;
        barn_append_char_to_allocated_string(last_tok_value, '/');
        return;
    }

    if (lexer->is_comment_multiline == false)
        lexer->is_comment_multiline = true;
    else
    {
        barn_error_show_with_line(lexer,
            BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
            barn_get_element_from_array(lexer->file_lines, lexer->row),
            "opening another multiline comment when the old one isn't closed");
        exit(1);
    }
}

void 
barn_lexer_comment_multiline_close(barn_lexer_t* lexer)
{
    if (lexer->is_string == true)
    {
        char* last_tok_value = lexer->last_token->value;
        barn_append_char_to_allocated_string(last_tok_value, '*');
        return;
    }

    if (lexer->is_comment_multiline == true)
    {
        lexer->is_comment_multiline = false;
        lexer->is_space = true;

        barn_lexer_advance(lexer, 1);
    }
    else
    {
        barn_error_show_with_line(lexer,
            BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
            barn_get_element_from_array(lexer->file_lines, lexer->row),
            "there isn't any multiline comment that need to be closed");
        exit(1);
    }
}

void 
barn_lexer_create_string(barn_lexer_t* lexer)
{
    if (lexer->is_string)
    {
        lexer->is_string = false;
        return;
    }

    lexer->is_string = true;

    char* current_line  = barn_get_element_from_array(lexer->file_lines, lexer->row);
    barn_token_t* token = barn_create_token("", (char*)lexer->filename, current_line,
                                            lexer->col, lexer->row, BARN_TOKEN_STRING);

    barn_append_element_to_array(lexer->tokens, token);
}

void 
barn_lexer_add_char_to_string(barn_lexer_t* lexer)
{
    char* last_tok_value = lexer->last_token->value;

    if (lexer->curr_char != '\\')
    {
        barn_append_char_to_allocated_string(last_tok_value, lexer->curr_char);
        return;
    }

    barn_lexer_advance(lexer, 1);

    switch (lexer->curr_char)
    {
        case 't':
            barn_append_char_to_allocated_string(last_tok_value, '\\');
            barn_append_char_to_allocated_string(last_tok_value, 't');
            break;
        case 'n':
            barn_append_char_to_allocated_string(last_tok_value, '\\');
            barn_append_char_to_allocated_string(last_tok_value, 'n');
            break;
        case 'r':
            barn_append_char_to_allocated_string(last_tok_value, '\\');
            barn_append_char_to_allocated_string(last_tok_value, 'r');
            break;
        case 'b':
            barn_append_char_to_allocated_string(last_tok_value, '\\');
            barn_append_char_to_allocated_string(last_tok_value, 'b');
            break;
        case 'e':
            barn_append_char_to_allocated_string(last_tok_value, '\\');
            barn_append_char_to_allocated_string(last_tok_value, 'e');
            break;
        case '\\':
            barn_append_char_to_allocated_string(last_tok_value, '\\');
            break;
        case '"':
            barn_append_char_to_allocated_string(last_tok_value, '"');
            break;
        default:
            barn_error_show_with_line(lexer,
                BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 2, true, 
                barn_get_element_from_array(lexer->file_lines, lexer->row),  
                "unknown escape sequence '\\%c'", lexer->curr_char);
            exit(1);
            break;
    }
}

char*
barn_lexer_collect_to_char_token(barn_lexer_t* lexer)
{
    barn_lexer_advance(lexer, 1);
    char value_of_token[2] = { lexer->curr_char, '\0' };
    
    if (value_of_token[0] == '\0')
    {
        barn_error_show_with_line(lexer,
            BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
            barn_get_element_from_array(lexer->file_lines, lexer->row),  
            "expected character instead of EOF");
        exit(1);
    }

    return barn_duplicate_string(value_of_token);
}

void 
barn_lexer_create_char(barn_lexer_t* lexer)
{
    if (lexer->next_char == '\'')
    {
        barn_error_show_with_line(lexer,
            BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
            barn_get_element_from_array(lexer->file_lines, lexer->row),  
            "expected an character between ''");
        exit(1);
    }

    char* value_of_char = barn_lexer_collect_to_char_token(lexer);
    barn_lexer_advance(lexer, 1);

    if (lexer->curr_char != '\'')
    {
        barn_error_show_with_line(lexer,
            BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
            barn_get_element_from_array(lexer->file_lines, lexer->row),  
            "expected ' to close character token");
        exit(1);
    }

    char* current_line  = barn_get_element_from_array(lexer->file_lines, lexer->row);
    barn_token_t* token = barn_create_token(value_of_char, (char*)lexer->filename, current_line,
                                            lexer->col, lexer->row, BARN_TOKEN_CHAR);

    barn_append_element_to_array(lexer->tokens, token);
    lexer->is_space = false;
}

bool
barn_lexer_is_char_identifier(barn_lexer_t* lexer)
{
    static const char* identifiers_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ_@$";

    for (int i = 0; i < strlen(identifiers_characters); i++)
        if (identifiers_characters[i] == lexer->curr_char)
            return true;

    return false;
}

bool
barn_lexer_is_char_number(barn_lexer_t* lexer)
{
    static const char* numbers_characters = "0123456789";

    for (int i = 0; i < strlen(numbers_characters); i++)
        if (numbers_characters[i] == lexer->curr_char)
            return true;

    return false;
}

bool
barn_lexer_create_identifier_last_token_null(barn_lexer_t* lexer, char* current_line)
{
    if (lexer->last_token != NULL)
        return false;

    barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                            current_line, lexer->col, lexer->row, BARN_TOKEN_IDENTIFIER);

    barn_append_element_to_array(lexer->tokens, token);
    lexer->is_space = false;

    return true;
}

bool
barn_lexer_create_identifier_no_space(barn_lexer_t* lexer, char* current_line)
{
    if (lexer->is_space == true)
        return false;

    if (lexer->last_token->kind == BARN_TOKEN_IDENTIFIER)
    {
        /* Append current char to last token value (hope it's allocated lol) */
        barn_append_char_to_allocated_string(lexer->last_token->value, lexer->curr_char);
    }
    else
    {
        barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                                current_line, lexer->col, lexer->row, BARN_TOKEN_IDENTIFIER);

        barn_append_element_to_array(lexer->tokens, token);
        lexer->is_space = false;
    }

    return true;
}

void
barn_lexer_create_identifier(barn_lexer_t* lexer)
{
    char* current_line = barn_get_element_from_array(lexer->file_lines, lexer->row);

    if (barn_lexer_create_identifier_last_token_null(lexer, current_line))
        return;

    if (barn_lexer_create_identifier_no_space(lexer, current_line))
        return;

    barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                            current_line, lexer->col, lexer->row, BARN_TOKEN_IDENTIFIER);

    barn_append_element_to_array(lexer->tokens, token);
    lexer->is_space = false;
}

bool
barn_lexer_create_number_last_token_null(barn_lexer_t* lexer, char* current_line)
{
    if (lexer->last_token != NULL)
        return false;

    barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                            current_line, lexer->col, lexer->row, BARN_TOKEN_INT);

    barn_append_element_to_array(lexer->tokens, token);
    lexer->is_space = false;

    return true; 
}

bool
barn_lexer_create_number_no_space(barn_lexer_t* lexer, char* current_line)
{
    if (lexer->is_space == true)
        return false;

    if (lexer->last_token->kind == BARN_TOKEN_INT        || 
        lexer->last_token->kind == BARN_TOKEN_FLOAT      ||
        lexer->last_token->kind == BARN_TOKEN_IDENTIFIER)
        barn_append_char_to_allocated_string(lexer->last_token->value, lexer->curr_char);
    else if (lexer->last_token->kind == BARN_TOKEN_DOT) 
    {
        barn_append_char_to_allocated_string(lexer->last_token->value, lexer->curr_char);
        lexer->last_token->kind = BARN_TOKEN_FLOAT;
    }
    else if (lexer->last_token->kind == BARN_TOKEN_MINUS)
    {
        char* neg_value = lexer->last_token->value;
        barn_append_char_to_allocated_string(neg_value, lexer->curr_char);

        lexer->last_token->kind = BARN_TOKEN_INT;
    } 
    else
    {
        barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                            current_line, lexer->col, lexer->row, BARN_TOKEN_INT);

        barn_append_element_to_array(lexer->tokens, token);
    }
    
    lexer->is_space = false;
    return true;
}

void 
barn_lexer_create_number(barn_lexer_t* lexer)
{
    char* current_line = barn_get_element_from_array(lexer->file_lines, lexer->row);

    if (barn_lexer_create_number_last_token_null(lexer, current_line))
        return;

    if (barn_lexer_create_number_no_space(lexer, current_line))
        return;

    barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                            current_line, lexer->col, lexer->row, BARN_TOKEN_INT);

    barn_append_element_to_array(lexer->tokens, token);
    lexer->is_space = false;
}

bool 
barn_lexer_create_float_last_token_no_null(barn_lexer_t* lexer, char* current_line)
{
    if (lexer->last_token == NULL)
        return false;

    if (lexer->last_token->kind == BARN_TOKEN_INT && lexer->is_space == false)
    {
        barn_append_char_to_allocated_string(lexer->last_token->value, lexer->curr_char);
        lexer->last_token->kind = BARN_TOKEN_FLOAT;
    }
    else if (lexer->last_token->kind == BARN_TOKEN_FLOAT && lexer->is_space == false)
    {
        barn_error_show_with_line(lexer,
            BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
            barn_get_element_from_array(lexer->file_lines, lexer->row),
            "can't add another '.' to float");
        exit(1);
    } 
    else 
    {
        if (lexer->next_char == '.')
        {
            /* TODO: Call barn_lexer_detect_symbol function and create
            * a new symbol token */
            barn_lexer_symbol_t symbol = barn_lexer_create_symbol(lexer);

            if (symbol.symbol_kind != BARN_TOKEN_NONE)
            {
                char* current_line  = barn_get_element_from_array(lexer->file_lines, lexer->row);
                barn_token_t* token = barn_create_token(barn_duplicate_string(symbol.symbol_value), (char*)lexer->filename, 
                                                        current_line, lexer->col, lexer->row, symbol.symbol_kind);

                barn_append_element_to_array(lexer->tokens, token);
            }
        }
        else
        {
            barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                                    current_line, lexer->col, lexer->row, BARN_TOKEN_DOT);

            barn_append_element_to_array(lexer->tokens, token);
        }
    }

    lexer->is_space = false;
    return true;
}

void
barn_lexer_create_float_number(barn_lexer_t* lexer)
{
    char* current_line = barn_get_element_from_array(lexer->file_lines, lexer->row);

    if (barn_lexer_create_float_last_token_no_null(lexer, current_line))
        return;

    if (lexer->next_char == '.')
    {
        /* TODO: Call barn_lexer_detect_symbol function and create
         * a new symbol token */
        barn_lexer_symbol_t symbol = barn_lexer_create_symbol(lexer);

        if (symbol.symbol_kind != BARN_TOKEN_NONE)
        {
            char* current_line  = barn_get_element_from_array(lexer->file_lines, lexer->row);
            barn_token_t* token = barn_create_token(barn_duplicate_string(symbol.symbol_value), (char*)lexer->filename, 
                                                    current_line, lexer->col, lexer->row, symbol.symbol_kind);

            barn_append_element_to_array(lexer->tokens, token);
        }
    }
    else
    {
        barn_token_t* token = barn_create_token(barn_create_string_from_char(lexer->curr_char), (char*)lexer->filename, 
                                                current_line, lexer->col, lexer->row, BARN_TOKEN_DOT);

        barn_append_element_to_array(lexer->tokens, token);
        lexer->is_space = false; 
    }
}

barn_lexer_symbol_t
barn_lexer_create_symbol(barn_lexer_t* lexer)
{
    barn_lexer_symbol_t symbol = {
        .symbol_value = "none",
        .symbol_kind  = BARN_TOKEN_NONE
    };

    switch (lexer->curr_char)
    {
        case '(':
        {
            symbol.symbol_value = "(";
            symbol.symbol_kind  = BARN_TOKEN_OPENPARENT;
            return symbol;
        }
        case ')':
        {
            symbol.symbol_value = ")";
            symbol.symbol_kind  = BARN_TOKEN_CLOSEPARENT;
            return symbol;
        }
        case '[':
        {
            symbol.symbol_value = "[";
            symbol.symbol_kind  = BARN_TOKEN_OPENBRACKET;
            return symbol;
        }
        case ']':
        {
            symbol.symbol_value = "]";
            symbol.symbol_kind  = BARN_TOKEN_CLOSEBRACKET;
            return symbol;
        }
        case '{':
        {
            symbol.symbol_value = "{";
            symbol.symbol_kind  = BARN_TOKEN_OPENBRACE;
            return symbol;
        }
        case '}':
        {
            symbol.symbol_value = "}";
            symbol.symbol_kind  = BARN_TOKEN_CLOSEBRACE;
            return symbol;
        }
        case '.':
        {
            if (lexer->next_char == '.')
            {
                barn_lexer_advance(lexer, 1);
                if (lexer->next_char == '.')
                {
                    barn_lexer_advance(lexer, 1);
                    symbol.symbol_value = "...";
                    symbol.symbol_kind  = BARN_TOKEN_TRIPLEDOT;
                    return symbol;    
                }
                else
                {
                    barn_error_show_with_line(lexer,
                        BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
                        barn_get_element_from_array(lexer->file_lines, lexer->row),
                        "expected '.' after '..' to create TRIPLEDOT token");
                    exit(1);
                }
            }

            return symbol;
        }
        case ';':
        {
            symbol.symbol_value = ";";
            symbol.symbol_kind  = BARN_TOKEN_SEMICOL;
            return symbol;
        }
        case '%':
        {
            symbol.symbol_value = "%";
            symbol.symbol_kind  = BARN_TOKEN_MOD;
            return symbol;
        }
        case '|':
        {
            if (lexer->next_char == '|')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "||";
                symbol.symbol_kind  = BARN_TOKEN_OROR;
                return symbol;
            }

            barn_error_show_with_line(lexer,
                BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
                barn_get_element_from_array(lexer->file_lines, lexer->row),
                "expected '||' to create OR token");
            exit(1);
        }
        case '&':
        {
            if (lexer->next_char == '&')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "&&";
                symbol.symbol_kind  = BARN_TOKEN_OROR;
                return symbol;
            }

            barn_error_show_with_line(lexer,
                BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
                barn_get_element_from_array(lexer->file_lines, lexer->row),
                "expected '&&' to create OR token");
            exit(1);
        }
        case '!':
        {
            if (lexer->next_char == '=')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "!=";
                symbol.symbol_kind  = BARN_TOKEN_NEQ;
                return symbol;
            }
            
            symbol.symbol_value = "!";
            symbol.symbol_kind  = BARN_TOKEN_BANG;
            return symbol;
        }
        case '+':
        {        
            if (lexer->next_char == '=')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "+=";
                symbol.symbol_kind  = BARN_TOKEN_PLUSASN;
                return symbol;
            } 
            else if (lexer->next_char == '+')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "++";
                symbol.symbol_kind  = BARN_TOKEN_INCREMENTATION;
                return symbol;
            }

            symbol.symbol_value = "+";
            symbol.symbol_kind  = BARN_TOKEN_PLUS;
            return symbol;
        }
        case '-':
        {
            if (lexer->next_char == '=')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "-=";
                symbol.symbol_kind  = BARN_TOKEN_MINUSASN;
                return symbol;
            } 
            else if (lexer->next_char == '>')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "->";
                symbol.symbol_kind  = BARN_TOKEN_ARROW;
                return symbol;
            } 
            else if (lexer->next_char == '-')
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "--";
                symbol.symbol_kind  = BARN_TOKEN_DECREMENTATION;
                return symbol;
            }

            symbol.symbol_value = "-";
            symbol.symbol_kind  = BARN_TOKEN_MINUS;
            return symbol;
        }
        case '*':
        {
            if (lexer->next_char == '=') 
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "*=";
                symbol.symbol_kind  = BARN_TOKEN_MULASN;
                return symbol;
            }

            symbol.symbol_value = "*";
            symbol.symbol_kind  = BARN_TOKEN_MUL;
            return symbol;
        }
        case '/':
        {
            if (lexer->next_char == '=') 
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "/=";
                symbol.symbol_kind  = BARN_TOKEN_DIVASN;
                return symbol;
            }

            symbol.symbol_value = "/";
            symbol.symbol_kind  = BARN_TOKEN_DIV;
            return symbol;
        }
        case '>':
        {
            if (lexer->next_char == '=') 
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = ">=";
                symbol.symbol_kind  = BARN_TOKEN_GTE;
                return symbol;
            }

            symbol.symbol_value = ">";
            symbol.symbol_kind  = BARN_TOKEN_GT;
            return symbol;
        }
        case '<':
        {
            if (lexer->next_char == '=') 
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "<=";
                symbol.symbol_kind  = BARN_TOKEN_LTE;
                return symbol;
            }

            symbol.symbol_value = "<";
            symbol.symbol_kind  = BARN_TOKEN_LT;
            return symbol;
        }
        case '=':
        {
            if (lexer->next_char == '=') 
            {
                barn_lexer_advance(lexer, 1);
                symbol.symbol_value = "==";
                symbol.symbol_kind  = BARN_TOKEN_EQ;
                return symbol;
            }

            symbol.symbol_value = "=";
            symbol.symbol_kind  = BARN_TOKEN_ASN;
            return symbol;
        }
	}
    return symbol;
}

void 
barn_lexer_main(barn_lexer_t* lexer)
{
    for (lexer->index = 0; lexer->index < strlen(lexer->file_content); lexer->index++)
    {
        barn_lexer_advance(lexer, 0);
    
        if (lexer->curr_char == '\n')
            barn_lexer_new_line(lexer);
        else if (lexer->curr_char == '\r')
            continue;
        else if (lexer->curr_char == ' ')
            barn_lexer_space(lexer);
        else if (lexer->curr_char == '/' && lexer->next_char == '/')
            barn_lexer_comment_inline(lexer);
        else if (lexer->curr_char == '/' && lexer->next_char == '*')
            barn_lexer_comment_multiline_open(lexer);
        else if (lexer->curr_char == '*' && lexer->next_char == '/')
            barn_lexer_comment_multiline_close(lexer);
        else if (lexer->is_comment_inline || lexer->is_comment_multiline)
            continue;
        else if (lexer->curr_char == '"')
            barn_lexer_create_string(lexer);
        else if (lexer->is_string)
        {
            barn_lexer_add_char_to_string(lexer);
            continue;
        }
        else if (lexer->curr_char == '\'')
            barn_lexer_create_char(lexer);
        else if (barn_lexer_is_char_identifier(lexer) && lexer->is_string == false)
            barn_lexer_create_identifier(lexer);
        else if (barn_lexer_is_char_number(lexer))
            barn_lexer_create_number(lexer);
        else if (lexer->curr_char == '.')
            barn_lexer_create_float_number(lexer);
        else
        {
            barn_lexer_symbol_t symbol = barn_lexer_create_symbol(lexer);

            if (symbol.symbol_kind != BARN_TOKEN_NONE)
            {
                char* current_line  = barn_get_element_from_array(lexer->file_lines, lexer->row);
                barn_token_t* token = barn_create_token(barn_duplicate_string(symbol.symbol_value), (char*)lexer->filename, 
                                                        current_line, lexer->col, lexer->row, symbol.symbol_kind);

                barn_append_element_to_array(lexer->tokens, token);
                lexer->is_space = false;
            }
            else
            {
                barn_error_show_with_line(lexer,
                    BARN_SYNTAX_ERROR, (char*)lexer->filename, lexer->row, lexer->col - 1, true, 
                    barn_get_element_from_array(lexer->file_lines, lexer->row),
                    "unknown use of character: \'%c\' (%d)", 
                        lexer->curr_char, lexer->curr_char);
                exit(1);
            }
        }
    }
}

void 
barn_lexer_show_all_tokens(barn_lexer_t* lexer)
{
    for (int i = 0; i < lexer->tokens->length; i++)
        barn_token_print((barn_token_t*)barn_get_element_from_array(lexer->tokens, i));
}
