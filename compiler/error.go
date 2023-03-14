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

import "fmt"

// Error codes
const (
	ARGUMENT_ERROR = iota
	FILE_ERROR
	SYNTAX_ERROR
	PARSER_ERROR
	UNDEFINED_ERROR
	UNKNOWN_ERROR
	COMPILER_ERROR
	OVERFLOW_ERROR
)

// Error code to string
func barn_error_code_to_string(code int) string {
	switch code {
	case ARGUMENT_ERROR:
		return "ArgumentError"
	case FILE_ERROR:
		return "FileError"
	case SYNTAX_ERROR:
		return "SyntaxError"
	case PARSER_ERROR:
		return "ParserError"
	case UNDEFINED_ERROR:
		return "UndefinedError"
	case UNKNOWN_ERROR:
		return "UnknownError"
	case COMPILER_ERROR:
		return "CompilerError"
	case OVERFLOW_ERROR:
		return "OverflowError"
	}

	return "UnknownError"
}

func barn_error_show_with_line(code int, message string, filename string, row int, col int, is_line bool, line string) {
	fmt.Printf("%s%s%s: %s:%d:%d: %s\n",
		get_color(Red),
		barn_error_code_to_string(code),
		get_color(Reset),
		filename,
		row,
		col,
		message)

	if is_line == true {
		if row != 1 {
			fmt.Printf("%s%d %s| %s...%s\n", get_color(Green), row-1, get_color(Gray), get_color(White), get_color(Reset))
		}
		fmt.Printf("%s%d %s| %s", get_color(Green), row, get_color(Gray), get_color(White))
		get_color(Gray)
		for i := 0; i < len(line); i++ {
			if i == col {
				fmt.Printf("%s", get_color(Green))
			}

			if line[i] == ' ' || line[i] == '(' || line[i] == '|' || line[i] == ')' || line[i] == '{' || line[i] == '}' || line[i] == '[' || line[i] == ']' || line[i] == ']' || line[i] == ':' {
				fmt.Printf("%s", get_color(Gray))
			}

			fmt.Printf("%c", line[i])
		}
		fmt.Printf("%s\n", get_color(Reset))
		fmt.Printf("%s%d %s| %s...%s\n\n", get_color(Green), row+1, get_color(Gray), get_color(White), get_color(Reset))
	}

}

func barn_error_show(code int, message string) {
	fmt.Printf("%s%s%s: %s\n",
		get_color(Red),
		barn_error_code_to_string(code),
		get_color(Reset),
		message)
}
