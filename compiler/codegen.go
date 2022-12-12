package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

const (
	C = iota
	NASM
	FASM
)

type Codegen struct {
	parser *Parser
	tab    int

	/* C Codegen */
	cxx_code   string
	cxx_header string

	/* NASM Linux x86_64 Codegen */
	nasm_file_header  string
	nasm_section_data string
	nasm_section_bss  string
	nasm_section_text string

	/* FASM Linux x86_64 Codegen */
	fasm_section_data string
	fasm_section_bss  string
	fasm_section_text string
}

func (codegen *Codegen) get_code(codegen_type int) (string, string) {
	switch codegen_type {
	case C:
		return fmt.Sprintf("%s\n%s", codegen.cxx_header, codegen.cxx_code), "c_out.cxx"
	case FASM:
		return "fasm", "fasm_out.asm"
	case NASM:
		return fmt.Sprintf("%s\n\nsection .data%s\nsection .bss%s\nsection .text%s", codegen.nasm_file_header, codegen.nasm_section_data, codegen.nasm_section_bss, codegen.nasm_section_text), "nasm_out.asm"
	}
	return "function (codegen *Codegen) get_code(): Unknown codegen_type, compiler error", "out.txt"
}

// Helps with convertion barn types to basic c types
func barn_types_to_cxx_types(barn_type BarnTypes) string {
	switch barn_type {
	case BARN_CHAR:
		return "char"
	case BARN_INTREGER:
		return "int"
	case BARN_FLOAT:
		return "float"
	case BARN_STR:
		return "std::string"
	case BARN_BOOL:
		return "bool"
	case -1:
		return "void"
	}

	barn_error_show(COMPILER_ERROR, "serious compiler error in function `barn_types_to_cxx_types()` in file `codegen.go`")
	os.Exit(1)
	return "this will never appers so i can write here anything ;)"
}

func gen_tab(tabs int) string {
	to_ret := ""
	for i := 0; i < tabs; i++ {
		to_ret += "	"
	}
	return to_ret
}

func generate_variable_declaration(node *NodeAST, codegen *Codegen) string {
	variable := node
	to_ret := ""
	if variable.variable_fn_call_value == true {
		to_ret += fmt.Sprintf("%s %s = %s",
			barn_types_to_cxx_types(variable.variable_type),
			variable.variable_name,
			strings.Split(variable.variable_value, "	")[1])
	} else {
		if variable.variable_is_arg == false {
			if variable.variable_type == BARN_STR {
				to_ret += fmt.Sprintf("%s %s = \"%s\"",
					barn_types_to_cxx_types(variable.variable_type),
					variable.variable_name,
					variable.variable_value)
			} else if variable.variable_type == BARN_CHAR {
				to_ret += fmt.Sprintf("%s %s = '%s'",
					barn_types_to_cxx_types(variable.variable_type),
					variable.variable_name,
					variable.variable_value)
			} else {
				to_ret += fmt.Sprintf("%s %s = %s",
					barn_types_to_cxx_types(variable.variable_type),
					variable.variable_name,
					variable.variable_value)
			}
		}
	}
	return to_ret
}

func generate_variable_modify(node *NodeAST, codegen *Codegen) string {
	to_ret := ""
	if node.node_kind == VARIABLE_ASSIGNMENT {
		to_ret += fmt.Sprintf("%s = %s",
			node.variable_assignment_name,
			node.variable_assignment_value)
	} else if node.node_kind == VARIABLE_PLUS_ASSIGNMENT {
		to_ret += fmt.Sprintf("%s += %s",
			node.variable_plus_assignment_name,
			node.variable_plus_assignment_value)
	} else if node.node_kind == VARIABLE_MINUS_ASSIGNMENT {
		to_ret += fmt.Sprintf("%s -= %s",
			node.variable_minus_assignment_name,
			node.variable_minus_assignment_value)
	} else if node.node_kind == VARIABLE_MUL_ASSIGNMENT {
		to_ret += fmt.Sprintf("%s *= %s",
			node.variable_mul_assignment_name,
			node.variable_mul_assignment_value)
	} else if node.node_kind == VARIABLE_DIV_ASSIGNMENT {
		to_ret += fmt.Sprintf("%s /= %s",
			node.variable_div_assignment_name,
			node.variable_div_assignment_value)
	} else if node.node_kind == VARIABLE_DECREMENTATION {
		to_ret += fmt.Sprintf("%s--", node.variable_name)
	} else if node.node_kind == VARIABLE_INCREMENTATION {
		to_ret += fmt.Sprintf("%s++", node.variable_name)
	}
	return to_ret
}

func generate_code_cxx_function_body_nodes(node *NodeAST, codegen *Codegen) {
	if node.node_kind == FUNCTION_CALL {
		if node.call_name == "__code__" {
			codegen.cxx_code += fmt.Sprintf("/* __code__ %s */\n", codegen.parser.lex.filename)
			codegen.cxx_code += node.call_args[0].value
			codegen.cxx_code += "\n"
			codegen.cxx_code += "/* __code__ end */\n"
		} else {
			codegen.cxx_code += gen_tab(codegen.tab)
			codegen.cxx_code += fmt.Sprintf("%s(", node.call_name)
			if 0 == len(node.call_args) {
				codegen.cxx_code += ");\n"
			} else {
				for k := 0; k < len(node.call_args); k++ {
					pass_argument_value := node.call_args[k]
					if pass_argument_value.is_var == true {
						codegen.cxx_code += pass_argument_value.value
					} else {
						if pass_argument_value.type_arg == BARN_STR {
							codegen.cxx_code += "\""
							codegen.cxx_code += pass_argument_value.value
							codegen.cxx_code += "\""
						} else if pass_argument_value.type_arg == BARN_CHAR {
							codegen.cxx_code += "'"
							codegen.cxx_code += pass_argument_value.value
							codegen.cxx_code += "'"
						} else if pass_argument_value.type_arg == BARN_FLOAT {
							codegen.cxx_code += "(float)"
							codegen.cxx_code += pass_argument_value.value
						} else {
							codegen.cxx_code += pass_argument_value.value
						}
					}

					if (k + 1) != len(node.call_args) {
						codegen.cxx_code += ", "
					} else {
						codegen.cxx_code += ");\n"
					}
				}
			}
		}
	} else if node.node_kind == VARIABLE_DECLARATION {
		codegen.cxx_code += gen_tab(codegen.tab)
		variable := node
		if variable.variable_fn_call_value == true {
			codegen.cxx_code += fmt.Sprintf("%s %s = %s",
				barn_types_to_cxx_types(variable.variable_type),
				variable.variable_name,
				strings.Split(variable.variable_value, "	")[1])
		} else {
			if variable.variable_is_arg == false {
				if variable.variable_type == BARN_STR {
					codegen.cxx_code += fmt.Sprintf("%s %s = \"%s\";\n",
						barn_types_to_cxx_types(variable.variable_type),
						variable.variable_name,
						variable.variable_value)
				} else if variable.variable_type == BARN_CHAR {
					codegen.cxx_code += fmt.Sprintf("%s %s = '%s';\n",
						barn_types_to_cxx_types(variable.variable_type),
						variable.variable_name,
						variable.variable_value)
				} else {
					codegen.cxx_code += fmt.Sprintf("%s %s = %s;\n",
						barn_types_to_cxx_types(variable.variable_type),
						variable.variable_name,
						variable.variable_value)
				}
			}
		}
	} else if node.node_kind == VARIABLE_ASSIGNMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s = %s;\n",
			node.variable_assignment_name,
			node.variable_assignment_value)
	} else if node.node_kind == VARIABLE_PLUS_ASSIGNMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s += %s;\n",
			node.variable_plus_assignment_name,
			node.variable_plus_assignment_value)
	} else if node.node_kind == VARIABLE_MINUS_ASSIGNMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s -= %s;\n",
			node.variable_minus_assignment_name,
			node.variable_minus_assignment_value)
	} else if node.node_kind == VARIABLE_MUL_ASSIGNMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s *= %s;\n",
			node.variable_mul_assignment_name,
			node.variable_mul_assignment_value)
	} else if node.node_kind == VARIABLE_DIV_ASSIGNMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s /= %s;\n",
			node.variable_div_assignment_name,
			node.variable_div_assignment_value)
	} else if node.node_kind == FUNCTION_RETURN {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("return %s;\n",
			node.function_return_value)
	} else if node.node_kind == IF_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("if (%s) {\n", node.if_condition)
		codegen.tab += 1
	} else if node.node_kind == ELSE_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("else {\n")
		codegen.tab += 1
	} else if node.node_kind == ELSE_IF_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("else if (%s) {\n", node.else_if_condition)
		codegen.tab += 1
	} else if node.node_kind == WHILE_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("while (%s) {\n", node.while_condition)
		codegen.tab += 1
	} else if node.node_kind == END_STATEMENT {
		codegen.tab -= 1
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += "}\n"
	} else if node.node_kind == BREAK_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += "break;\n"
	} else if node.node_kind == CONTINUE_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += "continue;\n"
	} else if node.node_kind == VARIABLE_DECREMENTATION {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s--;\n", node.variable_name)
	} else if node.node_kind == VARIABLE_INCREMENTATION {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("%s++;\n", node.variable_name)
	} else if node.node_kind == FOR_STATEMENT {
		codegen.cxx_code += gen_tab(codegen.tab)
		codegen.cxx_code += fmt.Sprintf("for (%s;%s;%s) {\n",
			generate_variable_declaration(node.for_var_declaration, codegen),
			node.for_condition,
			generate_variable_modify(node.for_var_operation, codegen))
	} else if node.node_kind == IMPORT_C {
		if strings.HasPrefix(node.import_c_header, "./") {
			codegen.cxx_header += fmt.Sprintf("#include \"%s\"\n", node.import_c_header)
		} else {
			codegen.cxx_header += fmt.Sprintf("#include <%s>\n", node.import_c_header)
		}
	} else {
		barn_error_show(
			COMPILER_ERROR,
			fmt.Sprintf("Generating with c don't support %s nodes, try diffrent compiling method like nasm or fasm",
				node.node_kind_str))
		os.Exit(1)
	}
}

func codegen_c(codegen *Codegen) {
	codegen.cxx_header += "#include \"" + get_barn_libs_directory() + "std-cxx/barn_header.hxx" + "\"\n\n"
	content, err := ioutil.ReadFile("./lib/std-c/barn-std.cxx")
	if err != nil {
		content, err = ioutil.ReadFile(get_barn_libs_directory() + "std-cxx/barn-std.cxx")
		if err != nil {
			fmt.Println(get_barn_libs_directory() + "std-c/barn-std.cxx" + " is not found")
		}
	}
	codegen.cxx_header += string(content)
	codegen.cxx_header += "\n"
	for i := 0; i < len(codegen.parser.nodes); i++ {
		if codegen.parser.nodes[i].node_kind == FUNCTION_DECLARATION {
			if codegen.parser.nodes[i].function_name == "main" {
				codegen.cxx_code += "int main("
			} else {
				codegen.cxx_code += fmt.Sprintf("__BARN_FUNCTION__ %s %s(", barn_types_to_cxx_types(codegen.parser.nodes[i].function_return), codegen.parser.nodes[i].function_name)
			}

			for j := 0; j < len(codegen.parser.nodes[i].function_args); j++ {
				c_type := barn_types_to_cxx_types(codegen.parser.nodes[i].function_args[j].type_arg)
				if j+1 == len(codegen.parser.nodes[i].function_args) {
					codegen.cxx_code += fmt.Sprintf(
						"%s %s",
						c_type,
						codegen.parser.nodes[i].function_args[j].name)
				} else {
					codegen.cxx_code += fmt.Sprintf(
						"%s %s, ",
						c_type,
						codegen.parser.nodes[i].function_args[j].name)
				}
			}
			codegen.cxx_code += ") {\n"
			codegen.tab += 1
			for j := 0; j < len(codegen.parser.nodes[i].function_body); j++ {
				generate_code_cxx_function_body_nodes(codegen.parser.nodes[i].function_body[j], codegen)
			}
			codegen.tab -= 1
			codegen.cxx_code += "}\n\n"

		} else if codegen.parser.nodes[i].node_kind == IMPORT_C {
			if strings.HasPrefix(codegen.parser.nodes[i].import_c_header, "./") {
				codegen.cxx_header += fmt.Sprintf("#include \"%s\"\n", codegen.parser.nodes[i].import_c_header)
			} else {
				codegen.cxx_header += fmt.Sprintf("#include <%s>\n", codegen.parser.nodes[i].import_c_header)
			}
		} else if codegen.parser.nodes[i].node_kind == VARIABLE_DECLARATION {
			variable := codegen.parser.nodes[i]
			if variable.variable_type == BARN_STR {
				codegen.cxx_code += fmt.Sprintf("__BARN_GLOBAL_VARIABLE__ %s %s = \"%s\";\n",
					barn_types_to_cxx_types(variable.variable_type),
					variable.variable_name,
					variable.variable_value)
			} else if variable.variable_type == BARN_CHAR {
				codegen.cxx_code += fmt.Sprintf("__BARN_GLOBAL_VARIABLE__ %s %s = '%s';\n",
					barn_types_to_cxx_types(variable.variable_type),
					variable.variable_name,
					variable.variable_value)
			} else {
				codegen.cxx_code += fmt.Sprintf("__BARN_GLOBAL_VARIABLE__ %s %s = %s;\n",
					barn_types_to_cxx_types(variable.variable_type),
					variable.variable_name,
					variable.variable_value)
			}
		}
	}
}

func codegen_fasm(codegen *Codegen) {
	fmt.Println("Generating code with FASM is not supported yet")
	os.Exit(1)
}

func codegen_nasm(codegen *Codegen) {
	fmt.Println("Generating code with NASM is not supported yet")
	os.Exit(1)

	codegen.nasm_file_header = `bits   64
global _start`
	codegen.nasm_section_data = `
; .data`
	codegen.nasm_section_bss = `
; .bss`
	codegen.nasm_section_text = `
; .text
_start:
`
	if find_function(codegen.parser, "main") != nil {
		codegen.nasm_section_text += fmt.Sprintf("	call main\n")
	}
	codegen.nasm_section_text += `	mov rax, SYS_EXIT
	mov rsi, EXIT_SUCCESS
	syscall
	ret`
	for i := 0; i < len(codegen.parser.nodes); i++ {
		if codegen.parser.nodes[i].node_kind == FUNCTION_DECLARATION {
			codegen.nasm_section_text += "\n"
			codegen.nasm_section_text += fmt.Sprintf("%s: ; Function declaration of %s with return type %s and %d arguments\n",
				codegen.parser.nodes[i].function_name,
				codegen.parser.nodes[i].function_name,
				barn_types_to_cxx_types(codegen.parser.nodes[i].function_return),
				len(codegen.parser.nodes[i].function_args))
		}
	}
	codegen.nasm_file_header += `

%include "./lib/asm/nasm_include_std.asm"`
}

func codegen_start(parser *Parser, codegen_type int) *Codegen {
	var codegen Codegen
	codegen.parser = parser

	switch codegen_type {
	case C:
		codegen_c(&codegen)
	case FASM:
		codegen_fasm(&codegen)
	case NASM:
		codegen_nasm(&codegen)
	}

	return &codegen
}
