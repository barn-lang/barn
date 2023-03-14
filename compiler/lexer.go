/*
 *   Copyright (C) 2022-2023 Barn-Lang Organization
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

const identifer = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ_@$"
const number = "1234567890"

type Lexer struct {
	data       string
	filename   string
	data_lines [][]string
	tokens     []Token
	last_token *Token
	curr_char  byte
	next_char  byte

	filename_count int
	index          int
	col            int
	row            int

	is_char                    bool
	is_space                   bool
	is_string                  bool
	is_comment_inline          bool
	is_comment_multiline       bool
	is_negative_value_possible bool
}

// Function that are returning last token
func get_last_token(lex *Lexer) *Token {
	if len(lex.tokens) == 0 {
		return nil
	} else {
		return &lex.tokens[len(lex.tokens)-1]
	}
}

// Function that are returning next character
func get_next_char(lex *Lexer) byte {
	if (len(lex.data) - 1) == lex.index {
		return 0
	}

	return lex.data[lex.index+1]
}

// Function for skiping characters in lexer
func advance(lex *Lexer, count int) {
	lex.index += count
	lex.curr_char = lex.data[lex.index]
	lex.next_char = get_next_char(lex)
	lex.last_token = get_last_token(lex)

	lex.col++
}

// Function for creating inline comments if the lex.curr_char is equal to `/`
// and the lex.next_char is equal to `/`
func comment_inline_open(lex *Lexer) {
	// Is the string is opened then append to it current char
	if lex.is_string == true {
		buf := []byte(lex.last_token.value) // Create buffer with type []byte
		buf = append(buf, lex.curr_char)    // Append to the buffer another byte
		lex.last_token.value = string(buf)  // Convert buffer to string
	} else {
		// Just open the inline comment even if it's open just
		// open it ;)
		lex.is_comment_inline = true
		lex.is_space = true
	}
}

// Function for creating multiline comment if the lex.curr_char is equal to `/`
// and the lex.next_char is equal to `*`
func comment_multiline_open(lex *Lexer) {
	// Is the string is opened then append to it current char
	if lex.is_string == true {
		buf := []byte(lex.last_token.value) // Create buffer with type []byte
		buf = append(buf, lex.curr_char)    // Append to the buffer another byte
		lex.last_token.value = string(buf)  // Convert buffer to string
	} else {
		// If multiline comment is already opened then println out error
		// in other situation set multline property to `true`
		if lex.is_comment_multiline == false {
			lex.is_comment_multiline = true
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Opening another multiline comment when the old one isn't closed",
				lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
			os.Exit(1)
		}
	}
}

// Function for closing multiline comment if the lex.curr_char is equal to `*`
// and the lex.next_char is equal to `/`
func comment_multiline_close(lex *Lexer) {
	// Is the string is opened then append to it current char
	if lex.is_string == true {
		buf := []byte(lex.last_token.value) // Create buffer with type []byte
		buf = append(buf, lex.curr_char)    // Append to the buffer another byte
		lex.last_token.value = string(buf)  // Convert buffer to string
	} else {
		// If there's isn't any multiline commment that need to be ended then show
		// out error in other situation set multiline property to `false`
		if lex.is_comment_multiline == true {
			lex.is_comment_multiline = false
			lex.is_space = true
			advance(lex, 1)
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "There isn't any multiline comment that need to be closed",
				lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
			os.Exit(1)
		}
	}
}

// Function for creating identifier tokens
func creator_identifer(lex *Lexer) {
	if lex.last_token == nil {
		lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
			string(lex.curr_char), lex.filename,
			lex.col, lex.row, IDENTIFIER))
	} else {
		if lex.is_space == false {
			if lex.last_token.kind == IDENTIFIER {
				buf := []byte(lex.last_token.value) // Create buffer with type []byte
				buf = append(buf, lex.curr_char)    // Append to the buffer another byte
				lex.last_token.value = string(buf)  // Convert buffer to string
			} else {
				lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
					string(lex.curr_char), lex.filename,
					lex.col, lex.row, IDENTIFIER))
			}
		} else {
			lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
				string(lex.curr_char), lex.filename,
				lex.col, lex.row, IDENTIFIER))
		}
	}
	lex.is_space = false
}

// Function for creating a string token
func creator_string(lex *Lexer) {
	if lex.is_string {
		lex.is_string = false
	} else {
		lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
			"", lex.filename, lex.col,
			lex.row, STRING))
		lex.is_string = true
	}
}

// Function for appending characters to string token
func add_to_string(lex *Lexer) {
	if lex.curr_char == '\\' {
		advance(lex, 1)
		switch lex.curr_char {
		case 't':
			buf := []byte(lex.last_token.value) // Create buffer with type []byte
			buf = append(buf, byte('\\'))       // Append to the buffer another byte
			buf = append(buf, byte('t'))        // Append to the buffer another byte
			lex.last_token.value = string(buf)  // Convert buffer to string
		case 'n':
			buf := []byte(lex.last_token.value) // Create buffer with type []byte
			buf = append(buf, byte('\\'))       // Append to the buffer another byte
			buf = append(buf, byte('n'))        // Append to the buffer another byte
			lex.last_token.value = string(buf)  // Convert buffer to string
		case 'r':
			buf := []byte(lex.last_token.value) // Create buffer with type []byte
			buf = append(buf, byte('\\'))       // Append to the buffer another byte
			buf = append(buf, byte('r'))        // Append to the buffer another byte
			lex.last_token.value = string(buf)  // Convert buffer to string
		case '"':
			buf := []byte(lex.last_token.value) // Create buffer with type []byte
			buf = append(buf, '"')              // Append to the buffer another byte
			lex.last_token.value = string(buf)  // Convert buffer to string
		}
	} else {
		buf := []byte(lex.last_token.value) // Create buffer with type []byte
		buf = append(buf, lex.curr_char)    // Append to the buffer another byte
		lex.last_token.value = string(buf)  // Convert buffer to string
	}
}

// Function for creating a number token
func creator_number(lex *Lexer) {
	if lex.last_token == nil {
		lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
			string(lex.curr_char), lex.filename,
			lex.col, lex.row, INT))
		lex.is_space = false
	} else {
		if lex.is_space == false {
			if lex.last_token.kind == INT {
				buf := []byte(lex.last_token.value) // Create buffer with type []byte
				buf = append(buf, lex.curr_char)    // Append to the buffer another byte
				lex.last_token.value = string(buf)  // Convert buffer to string
				lex.is_space = false
			} else if lex.last_token.kind == FLOAT {
				buf := []byte(lex.last_token.value) // Create buffer with type []byte
				buf = append(buf, lex.curr_char)    // Append to the buffer another byte
				lex.last_token.value = string(buf)  // Convert buffer to string
				lex.is_space = false
			} else if lex.last_token.kind == IDENTIFIER && lex.is_space == false {
				buf := []byte(lex.last_token.value) // Create buffer with type []byte
				buf = append(buf, lex.curr_char)    // Append to the buffer another byte
				lex.last_token.value = string(buf)  // Convert buffer to string
				lex.last_token.kind = IDENTIFIER
				lex.is_space = false
			} else if lex.last_token.kind == DOT {
				buf := []byte(lex.last_token.value) // Create buffer with type []byte
				buf = append(buf, lex.curr_char)    // Append to the buffer another byte
				lex.last_token.value = string(buf)  // Convert buffer to string
				lex.last_token.kind = FLOAT
				lex.is_space = false
			} else if lex.last_token.kind == MINUS && lex.is_negative_value_possible == true {
				lex.is_negative_value_possible = false
				lex.tokens = lex.tokens[:len(lex.tokens)-1]
				lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
					"-"+string(lex.curr_char), lex.filename,
					lex.col, lex.row, INT))
				lex.is_space = false
			} else {
				lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
					string(lex.curr_char), lex.filename,
					lex.col, lex.row, INT))
				lex.is_space = false
			}
		} else {
			lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
				string(lex.curr_char), lex.filename,
				lex.col, lex.row, INT))
			lex.is_space = false
		}
	}
}

// Function for creating char tokens
func creator_char(lex *Lexer) {
	if lex.next_char == '\'' {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Expected an character between `'(here character)'`",
			lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
		os.Exit(1)
	} else {
		advance(lex, 1)
		value_of_char := lex.curr_char
		advance(lex, 1)
		if lex.curr_char == '\'' {
			lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
				string(value_of_char), lex.filename,
				lex.col - 2, lex.row, CHAR))
			lex.is_space = false
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected `'` to close char type",
				lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
			os.Exit(1)
		}
	}
}

// Function for creating float tokens
func creator_float(lex *Lexer) {
	if lex.last_token != nil {
		if lex.last_token.kind == NONE {
			lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
				".", lex.filename,
				lex.col, lex.row, DOT))
			lex.is_space = false
		} else {
			if lex.last_token.kind == INT && lex.is_space == false {
				buf := []byte(lex.last_token.value) // Create buffer with type []byte
				buf = append(buf, lex.curr_char)    // Append to the buffer another byte
				lex.last_token.value = string(buf)  // Convert buffer to string
				lex.last_token.kind = FLOAT
				lex.is_space = false
			} else if lex.last_token.kind == FLOAT && lex.is_space == false {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Can't add another `.` to float",
					lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
				os.Exit(1)
			} else {
				lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
					".", lex.filename,
					lex.col, lex.row, DOT))
				lex.is_space = false
			}
		}
	} else {
		lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
			".", lex.filename,
			lex.col, lex.row, DOT))
		lex.is_space = false
	}
}

// Function for detecting space ' '
func space(lex *Lexer) {
	if lex.is_string {
		buf := []byte(lex.last_token.value) // Create buffer with type []byte
		buf = append(buf, lex.curr_char)    // Append to the buffer another byte
		lex.last_token.value = string(buf)  // Convert buffer to string
		lex.is_space = false
		return
	}

	lex.is_space = true
}

// Function for detecting symbols in lexer
func detect_symbol(lex *Lexer) (bool, string, int) {
	switch lex.curr_char {
	case '(':
		return true, "(", OPENPARENT
	case ')':
		return true, ")", CLOSEPARENT
	case '[':
		return true, "[", OPENBRACKET
	case ']':
		return true, "]", CLOSEBRACKET
	case '{':
		return true, "{", OPENBRACE
	case '}':
		return true, "}", CLOSEBRACE
	case ',':
		return true, ",", COMMA
	case ':':
		return true, ":", COLON
	case '.':
		return true, ".", DOT
	case ';':
		return true, ";", SEMICOL
	case '%':
		return true, "%", MOD
	case '|':
		if lex.next_char == '|' {
			advance(lex, 1)
			return true, "||", OROR
		}
		// barn_error_show_with_line(
		// 	SYNTAX_ERROR, fmt.Sprintf("Expected '|' after '%c'", lex.curr_char),
		// 	lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
		// os.Exit(1)
		return true, "|", CONDITIONBLOCK
	case '&':
		if lex.next_char == '&' {
			advance(lex, 1)
			return true, "&&", ANDAND
		}
		barn_error_show_with_line(
			SYNTAX_ERROR, fmt.Sprintf("Expected '&' after '%c'", lex.curr_char),
			lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
		os.Exit(1)
	case '!':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "!=", NEQ
		}
		return true, "!", BANG
	case '+':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "+=", PLUSASN
		} else if lex.next_char == '+' {
			advance(lex, 1)
			return true, "++", INCREMENTATION
		}
		return true, "+", PLUS
	case '-':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "-=", MINUSASN
		} else if lex.next_char == '>' {
			advance(lex, 1)
			return true, "->", ARROW
		} else if lex.next_char == '-' {
			advance(lex, 1)
			return true, "--", DECREMENTATION
		}
		lex.is_negative_value_possible = true
		return true, "-", MINUS
	case '*':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "*=", MULASN
		}
		return true, "*", MUL
	case '/':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "/=", DIVASN
		}
		return true, "/", DIV
	case '>':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, ">=", GTE
		}
		return true, ">", GT
	case '<':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "<=", LTE
		}
		return true, "<", LT
	case '=':
		if lex.next_char == '=' {
			advance(lex, 1)
			return true, "==", EQ
		}
		return true, "=", ASN
	}

	return false, "", NONE
}

// Main lexer function
func lexer_start(data string, data_lines [][]string, filename string, filename_count int) *Lexer {
	// Create lexer stucture that will be returned to main function
	var lex Lexer

	// Initialize lexer structure
	lex.data = data
	lex.filename = filename
	lex.data_lines = data_lines
	lex.filename_count = filename_count
	lex.curr_char = 0
	lex.next_char = 0
	lex.is_space = false
	lex.is_string = false
	lex.is_comment_inline = false
	lex.is_comment_multiline = false
	lex.row = 1
	lex.col = 0

	// Loop of lexer
	for lex.index = 0; lex.index < len(lex.data); lex.index++ {
		advance(&lex, 0)

		if lex.curr_char == '\n' {
			lex.row += 1
			lex.col = 0
			lex.is_space = true
			lex.is_comment_inline = false
		} else if lex.curr_char == '\r' {
			continue
		} else if lex.curr_char == byte(9) { // Ignore these characters
			continue
		} else if lex.curr_char == ' ' {
			space(&lex)
		} else if lex.curr_char == '/' && lex.next_char == '/' {
			comment_inline_open(&lex)
		} else if lex.curr_char == '/' && lex.next_char == '*' {
			comment_multiline_open(&lex)
		} else if lex.curr_char == '*' && lex.next_char == '/' {
			comment_multiline_close(&lex)
		} else if lex.is_comment_inline || lex.is_comment_multiline {
			continue
		} else if lex.curr_char == '"' {
			creator_string(&lex)
		} else if lex.is_string {
			add_to_string(&lex)
		} else if lex.curr_char == '\'' {
			creator_char(&lex)
		} else if strings.Contains(identifer, string(lex.curr_char)) {
			creator_identifer(&lex)
		} else if strings.Contains(number, string(lex.curr_char)) {
			creator_number(&lex)
		} else if lex.curr_char == '.' {
			creator_float(&lex)
		} else {
			is_symbol, value_symbol, kind_symbol := detect_symbol(&lex)
			if is_symbol {
				lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
					value_symbol, lex.filename,
					lex.col, lex.row, kind_symbol))
				lex.is_space = false
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Unknown use of `%c` (%d)", lex.curr_char, int(lex.curr_char)),
					lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
				os.Exit(1)
			}
		}
	}

	lex.tokens = append(lex.tokens, create_token(lex.data_lines[0][lex.row - 1], lex.filename_count,
		"EOF", lex.filename,
		lex.col, lex.row, EOF))

	// for i := 0; i < len(lex.tokens); i++ {
	// 	print_token(lex.tokens[i])
	// }

	if lex.is_string {
		barn_error_show_with_line(
			SYNTAX_ERROR, "String is not ended expected `\"`",
			lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
		os.Exit(1)
	}

	if lex.is_char {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Char is not ended expected `'`",
			lex.filename, lex.row, lex.col-1, true, lex.data_lines[0][lex.row-1])
		os.Exit(1)
	}

	return &lex
}

func import_files(lexer *Lexer) *Lexer {
	for i := 0; i < len(lexer.tokens); i++ {
		if lexer.tokens[i].value == "@import" && lexer.tokens[i].kind == IDENTIFIER {
			if lexer.tokens[i+1].kind != EOF {
				if lexer.tokens[i+1].kind == STRING {
					content, err := ioutil.ReadFile(lexer.tokens[i+1].value)
					if err != nil {
						content, err = ioutil.ReadFile("./" + lexer.tokens[i+1].value)
						if err != nil {
							content, err = ioutil.ReadFile(get_barn_libs_directory() + lexer.tokens[i+1].value)
							if err != nil {
								barn_error_show_with_line(
									SYNTAX_ERROR, fmt.Sprintf("Import file is not found `%s`", lexer.tokens[i+1].value),
									lexer.filename, lexer.tokens[i].row, lexer.tokens[i].col-1,
									true, lexer.data_lines[0][lexer.tokens[i].row-1])
								os.Exit(1)
							}
						}
					}
					lexer_import := lexer_start(
						string(content),
						[][]string{strings.Split(string(content), "\n")},
						lexer.tokens[i+1].value, len(lexer.data_lines)-1)
					copy_tokens := lexer.tokens
					lexer_import.tokens = lexer_import.tokens[:len(lexer_import.tokens)-1]
					lexer.data_lines = append(lexer.data_lines, strings.Split(string(content), "\n"))
					lexer.tokens = lexer_import.tokens
					for j := 0; j < len(copy_tokens); j++ {
						lexer.tokens = append(lexer.tokens, copy_tokens[j])
					}
					i += len(lexer_import.tokens)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Expected import file name"),
					lexer.filename, lexer.tokens[i].row, lexer.tokens[i].col-1,
					true, lexer.data_lines[0][lexer.tokens[i].row-1])
				os.Exit(1)
			}
		} else {
			continue
		}
	}

	return lexer
}
