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
	"errors"
	"fmt"
	"os"
	"strings"
	"log"
)

func version() {
	fmt.Printf("Barn %s0.1v%s (BETA)\n", get_color(Green), get_color(Reset))
}

func help() {
	fmt.Printf("%sBarn Help:%s\n", get_color(Green), get_color(Reset))
	fmt.Printf("  - %sFlags%s\n", get_color(Yellow), get_color(Reset))
	fmt.Printf("    > %s--help%s show this message\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--version%s show version of barn compiler\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--nasm%s compile and write output of program in NASM\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--fasm%s compile and write output of program in FASM\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--c%s compile and write output of program in C\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--tokens%s show all tokens from lexer (for debug purposes)\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--time%s show lexer, parser and codegen process time\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--cflags%s argument that can add c flags to compilation\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--no-main%s don't show error about no main function\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--no-stdlib%s don't include barn_header.hxx\n", get_color(Cyan), get_color(Reset))
	fmt.Printf("    > %s--no-delete-cout%s don't delete generated source code\n", get_color(Cyan), get_color(Reset))
}

func main() {
	args := args_parser_start()

	if args.is_filename == true {
		if _, err := os.Stat(args.filename); errors.Is(err, os.ErrNotExist) {
			barn_error_show(FILE_ERROR, fmt.Sprintf("`%s` do not exists", args.filename))
			os.Exit(1)
		} else {
			file_value := get_file_value(args.filename)
			file_lines := [][]string{strings.Split(file_value, "\n")}

			time_compilation, is_time_compilation := register_time_compilation(args)
			lexer := lexer_start(file_value, file_lines, args.filename, 0)
			lexer = import_files(lexer)

			if args.is_flag("--tokens") || args.is_flag("-tk") {
				for i := 0; i < len(lexer.tokens); i++ {
					print_token(lexer.tokens[i]);
				}
			}

			parser := parser_start(lexer, &args)

			// Recognize codegen type
			codegen_type := C
			if args.is_flag("--nasm") || args.is_flag("-nasm") {
				codegen_type = NASM
			} else if args.is_flag("--fasm") || args.is_flag("-fasm") {
				codegen_type = FASM
			}

			codegen := codegen_start(parser, codegen_type)
			code, filename := codegen.get_code(codegen_type)

			calculate_time_compiliation(time_compilation, is_time_compilation)

			file, err := os.Create(filename)

			if err != nil {
				barn_error_show(FILE_ERROR, fmt.Sprintf("Compiler could not create file with name %s, maybe you are in directory for root", filename))
				os.Exit(1)
			}

			file.WriteString(code)

			if args.is_flag("--cflags") || args.is_flag("-cf") {
				if args.is_flag("--cflags") {
					cflags := "./c_out.cxx " + args.get_flag_by_index(args.get_flag_index("--cflags") + 1)
					splited_cflags := strings.Split(cflags, " ")

					_, stderr, exit_code := run_command("g++", splited_cflags...)
					if (exit_code != 0) {
						barn_error_show(COMPILER_ERROR, "Compiling file named `c_out.cxx` failed due to: ")
						fmt.Print(stderr)
					}
				} else {
					cflags := "./c_out.cxx " + args.get_flag_by_index(args.get_flag_index("-cf") + 1)
					splited_cflags := strings.Split(cflags, " ")

					_, stderr, exit_code := run_command("g++", splited_cflags...)
					if (exit_code != 0) {
						barn_error_show(COMPILER_ERROR, "Compiling file named `c_out.cxx` failed due to: ")
						fmt.Print(stderr)
					}
				}
			} else {
				_, stderr, exit_code := run_command("g++", "./c_out.cxx")
				if (exit_code != 0) {
					barn_error_show(COMPILER_ERROR, "Compiling file named `c_out.cxx` failed due to: ")
					fmt.Print(stderr)
				}
			}

			if args.is_flag("--no-delete-cout") == false {
				e := os.Remove("./c_out.cxx")
				if e != nil {
					log.Fatal(e)
				}
			}
		}
	} else {
		if len(args.flags) == 0 {
			fmt.Printf("See more in %s%s --help%s\n", get_color(Green), os.Args[0], get_color(Reset))
			version()
			os.Exit(1)
		} else {
			for _, flag := range args.flags {
				if flag == "--help" || flag == "-help" || flag == "--h" || flag == "-h" {
					help()
				} else if flag == "--version" || flag == "-version" || flag == "--v" || flag == "-v" {
					version()
				} else {
					barn_error_show(ARGUMENT_ERROR, fmt.Sprintf("Unknown use of `%s` flag", flag))
					os.Exit(1)
				}
			}
		}
	}
}
