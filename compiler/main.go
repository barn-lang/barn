package main

// TODO: for
// TODO: math operations function return

import (
	"errors"
	"fmt"
	"os"
	"strings"
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
			lexer := lexer_start(file_value, file_lines, args.filename)
			lexer = import_files(lexer)
			parser := parser_start(lexer)

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
