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
