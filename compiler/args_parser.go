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
	"os"
)

type ArgsParser struct {
	is_filename bool
	filename    string
	flags_len   int
	flags       []string
}

func (args_parser ArgsParser) is_flag(flag string) bool {
	for i := 0; i < len(args_parser.flags); i++ {
		if args_parser.flags[i] == flag {
			return true
		}
	}

	return false
}

func (args_parser ArgsParser) get_flag_index(flag string) int {
	for i := 0; i < len(args_parser.flags); i++ {
		if args_parser.flags[i] == flag {
			return i
		}
	}

	return -1
}

func (args_parser ArgsParser) get_flag_by_index(index int) string {
	if index == -1 { return "" }
	if index > len(args_parser.flags) { return "" }
	return args_parser.flags[index]
}

func args_parser_start() ArgsParser {
	var return_value ArgsParser

	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]

		if arg[0] == '-' {
			if arg[1] == '-' {
				return_value.flags = append(return_value.flags, arg)
				return_value.flags_len++
			} else {
				return_value.flags = append(return_value.flags, arg)
				return_value.flags_len++
			}
		} else {
			if return_value.is_filename == false {
				return_value.is_filename = true
				return_value.filename = arg
			} else {
				barn_error_show(ARGUMENT_ERROR, fmt.Sprintf("Unknown use of `%s` argument", arg))
				os.Exit(1)
			}
		}
	}

	return return_value
}
