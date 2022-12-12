package main

import (
	"fmt"
	"os"
)

type Parser struct {
	nodes      []*NodeAST
	curr_token *Token
	lex        *Lexer
	index      int

	is_function_opened        bool
	actual_function           *NodeAST
	functions                 []*NodeAST
	local_variables           []*NodeAST
	global_variables          []*NodeAST
	statement_open            int
	last_statement_opened     string
	if_statement_node         *NodeAST
	is_while_statement_opened int
	is_for_statement_opened   int
}

// func get_next_token(parser *Parser) *Token {
// 	if len(parser.lex.tokens) == parser.index {
// 		return nil
// 	} else {
// 		return parser.lex.tokens[parser.index+1]
// 	}
// }

// Function for parsing values
func parse_value(parser *Parser, expected_type BarnTypes) string {
	skip_token(parser, 1)
	do_we_continue := true
	expect_symbol := false
	expect_number := true
	parents := 0
	to_ret := ""
	if parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT {
		for do_we_continue {
			if (parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT) && expect_number == true {
				to_ret += parser.curr_token.value
				expect_symbol = true
				expect_number = false
				skip_token(parser, 1)
				continue
			} else if parser.curr_token.kind == IDENTIFIER && expect_number == true {
				if find_var := find_variable_both(parser, parser.curr_token.value); find_var != nil {
					if find_var.variable_type == BARN_INTREGER || find_var.variable_type == BARN_FLOAT {
						to_ret += parser.curr_token.value
						expect_symbol = true
						expect_number = false
						skip_token(parser, 1)
						continue
					} else {
						barn_error_show_with_line(
							UNDEFINED_ERROR, fmt.Sprintf("Expected variable with type `int` or `float` not `%s`", find_var.variable_type.as_string()),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				} else {
					barn_error_show_with_line(
						UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && expect_symbol == true {
				to_ret += parser.curr_token.value
				expect_symbol = false
				expect_number = true
				skip_token(parser, 1)
				continue
			} else if parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT {
				if parser.curr_token.kind == OPENPARENT {
					to_ret += parser.curr_token.value
					parents += 1
					expect_number = true
					expect_symbol = false
					skip_token(parser, 1)
					continue
				} else {
					if parents == 0 {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Unexpected use of `)`",
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else {
						to_ret += parser.curr_token.value
						parents -= 1
						expect_number = false
						expect_symbol = true
						skip_token(parser, 1)
						continue
					}
				}
			} else if expect_number == true {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected number",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			} else if expect_symbol == true {
				break
			} else {
				break
			}
		}
		if expected_type == BARN_INTREGER || expected_type == BARN_FLOAT {
			return to_ret
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Expected expression with type `int` or `float` not `%s`", expected_type.as_string()),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else if parser.curr_token.kind == IDENTIFIER {
		if parser.curr_token.value == "true" || parser.curr_token.value == "false" {
			if expected_type == BARN_BOOL {
				return parser.curr_token.value
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Expected expression with type `bool` not `%s`", expected_type.as_string()),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			to_ret_2 := parser.curr_token.value
			skip_token(parser, 1)
			if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && (expected_type == BARN_INTREGER || expected_type == BARN_FLOAT) {
				to_ret += to_ret_2
				expect_number = false
				expect_symbol = true
				for do_we_continue {
					if (parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT) && expect_number == true {
						to_ret += parser.curr_token.value
						expect_symbol = true
						expect_number = false
						skip_token(parser, 1)
						continue
					} else if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && expect_symbol == true {
						to_ret += parser.curr_token.value
						expect_symbol = false
						expect_number = true
						skip_token(parser, 1)
						continue
					} else if parser.curr_token.kind == IDENTIFIER && expect_number == true {
						if find_var := find_variable_both(parser, parser.curr_token.value); find_var != nil {
							if find_var.variable_type == BARN_INTREGER || find_var.variable_type == BARN_FLOAT {
								to_ret += parser.curr_token.value
								expect_symbol = true
								expect_number = false
								skip_token(parser, 1)
								continue
							} else {
								barn_error_show_with_line(
									UNDEFINED_ERROR, fmt.Sprintf("Expected variable with type `int` or `float` not `%s`", find_var.variable_type.as_string()),
									parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
									true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
								os.Exit(1)
							}
						} else {
							barn_error_show_with_line(
								UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						}
					} else if parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT {
						if parser.curr_token.kind == OPENPARENT {
							to_ret += parser.curr_token.value
							parents += 1
							expect_number = true
							expect_symbol = false
							skip_token(parser, 1)
							continue
						} else {
							if parents == 0 {
								barn_error_show_with_line(
									SYNTAX_ERROR, "Unexpected use of `)`",
									parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
									true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
								os.Exit(1)
							} else {
								to_ret += parser.curr_token.value
								parents -= 1
								expect_number = false
								expect_symbol = true
								skip_token(parser, 1)
								continue
							}
						}
					} else if expect_number == true {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Expected number",
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else if expect_symbol == true {
						break
					} else {
						break
					}
				}
				if expected_type == BARN_INTREGER || expected_type == BARN_FLOAT {
					return to_ret
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, fmt.Sprintf("Expected expression with type `int` or `float` not `%s`", expected_type.as_string()),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				skip_token(parser, -1)
				find_var := find_variable_both(parser, parser.curr_token.value)
				if find_var == nil {
					barn_error_show_with_line(
						UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				} else {
					if expected_type == find_var.variable_type {
						skip_token(parser, 1)
						return to_ret_2
					} else {
						barn_error_show_with_line(
							SYNTAX_ERROR, fmt.Sprintf("Expected expression with type `%s` not `%s`", expected_type.as_string(), find_variable_both(parser, parser.curr_token.value).variable_type.as_string()),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				}
			}
		}
	} else if parser.curr_token.kind == STRING {
		if expected_type == BARN_STR {
			return parser.curr_token.value
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Expected expression with type `string` not `%s`", expected_type.as_string()),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else if parser.curr_token.kind == CHAR {
		if expected_type == BARN_CHAR {
			return parser.curr_token.value
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Expected expression with type `char` not `%s`", expected_type.as_string()),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, fmt.Sprintf("Unexpected use of `%s`", kind_to_str(parser.curr_token.kind)),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
		return "none"
	}
	return "none"
}

// Function for skipping tokens
func skip_token(parser *Parser, count int) {
	parser.index += count
	parser.curr_token = &parser.lex.tokens[parser.index]
}

// Function that helps to detect is token a
// keyword
func is_keyword(value_token string) bool {
	switch value_token {
	case "fun":
		return true
	case "@import_c":
		return true
	case "@import":
		return true
	case "let":
		return true
	case "return":
		return true
	case "if":
		return true
	case "else":
		return true
	case "elif":
		return true
	case "while":
		return true
	case "continue":
		return true
	case "break":
		return true
	case "for":
		return true
	case "enum":
		return true
	}

	return false
}

func is_next_token_kind(parser *Parser, kind int) bool {
	if parser.curr_token.kind != EOF {
		skip_token(parser, 1)
		if parser.curr_token.kind == kind {
			return true
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Expected `%s`, but found `%s`", kind_to_str(kind), kind_to_str(parser.curr_token.kind)),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
		return false
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, fmt.Sprintf("Expected `%s`", kind_to_str(kind)),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
		return false
	}
}

func is_next_token_kind_safe(parser *Parser, kind int) bool {
	skip_token(parser, 1)
	if parser.curr_token.kind == kind {
		return true
	}
	return false
}

func is_token_represent_type(value_token string) BarnTypes {
	switch value_token {
	case "char":
		return BARN_CHAR
	case "int":
		return BARN_INTREGER
	case "float":
		return BARN_FLOAT
	case "string":
		return BARN_STR
	case "bool":
		return BARN_BOOL
	}

	return BARN_TYPE_NONE
}

func is_argument_defined(args []ArgsFunctionDeclaration, name string) bool {
	for i := 0; i < len(args); i++ {
		if args[i].name == name {
			return true
		}
	}

	return false
}

func parse_function_args(parser *Parser) []ArgsFunctionDeclaration {
	if is_next_token_kind_safe(parser, CLOSEPARENT) {
		return []ArgsFunctionDeclaration{}
	} else {
		var expecting_type bool = true
		var expecting_name bool = false
		var expecting_comma bool = false
		var to_return []ArgsFunctionDeclaration
		for i := parser.index; i < len(parser.lex.tokens); i++ {
			if parser.curr_token.kind == CLOSEPARENT && expecting_name != true {
				skip_token(parser, 1)
				break
			} else {
				if expecting_type == true && expecting_name == false && expecting_comma == false {
					if parser.curr_token.kind == IDENTIFIER {
						kind_argument := is_token_represent_type(parser.curr_token.value)
						if kind_argument == -1 {
							barn_error_show_with_line(
								SYNTAX_ERROR, fmt.Sprintf("`%s` is not a type, maybe `char`, `int` or `string`", parser.curr_token.value),
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						} else {
							var arg ArgsFunctionDeclaration
							arg.name = ""
							arg.type_arg = kind_argument
							to_return = append(to_return, arg)
							expecting_type = false
							expecting_name = true
							expecting_comma = false
							skip_token(parser, 1)
							continue
						}
					} else {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Expected `IDENTIFIER` that represents type",
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				} else if expecting_type == false && expecting_name == true && expecting_comma == false {
					if parser.curr_token.kind == IDENTIFIER {
						if is_argument_defined(to_return, parser.curr_token.value) == true {
							barn_error_show_with_line(
								SYNTAX_ERROR, fmt.Sprintf("Argument name `%s` is already taken", parser.curr_token.value),
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						} else {
							to_return[len(to_return)-1].name = parser.curr_token.value
							expecting_type = false
							expecting_comma = true
							expecting_name = false
							skip_token(parser, 1)
							continue
						}
					} else {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Expected `IDENTIFIER` that represents name of argument",
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				} else if expecting_type == false && expecting_name == false && expecting_comma == true {
					if parser.curr_token.kind == COMMA {
						expecting_type = true
						expecting_comma = false
						expecting_name = false
						skip_token(parser, 1)
						continue
					} else {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Expected `COMMA` (`,`) that seperate arguments of function",
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				}
			}
		}
		return to_return
	}
}

// Function for easier appending nodes to abstract
// syntax tree
func append_node(parser *Parser, node NodeAST) {
	if parser.is_function_opened == false {
		parser.nodes = append(parser.nodes, &node)
	} else {
		for i := 0; i < len(parser.nodes); i++ {
			if parser.nodes[i].node_kind == FUNCTION_DECLARATION && parser.nodes[i].function_name == parser.actual_function.function_name {
				parser.nodes[i].function_body = append(parser.nodes[i].function_body, &node)
			}
		}
	}
}

// Function for easier appending nodes to abstract
// syntax tree but as pointer
func append_node_ptr(parser *Parser, node *NodeAST) {
	if parser.is_function_opened == false {
		parser.nodes = append(parser.nodes, node)
	} else {
		for i := 0; i < len(parser.nodes); i++ {
			if parser.nodes[i].node_kind == FUNCTION_DECLARATION && parser.nodes[i].function_name == parser.actual_function.function_name {
				parser.nodes[i].function_body = append(parser.nodes[i].function_body, node)
			}
		}
	}
}

// Function for parsing function declarations in
// code
func parse_function_declaration(parser *Parser) {
	// var function_return BarnTypes
	if parser.is_function_opened == false {
		// Try to get function name
		if is_next_token_kind(parser, IDENTIFIER) {
			function_name := parser.curr_token.value // Saving it in variable so we can use it later

			if is_next_token_kind(parser, OPENPARENT) {
				function_args := parse_function_args(parser)

				if parser.curr_token.kind == CLOSEPARENT {
					skip_token(parser, 1)
				}

				var variable_return BarnTypes = BARN_TYPE_NONE
				var is_ok bool = false
				if parser.curr_token.kind == OPENBRACE {
					is_ok = true
				} else if parser.curr_token.kind == ARROW {
					skip_token(parser, 1)
					return_type := is_token_represent_type(parser.curr_token.value)
					if return_type == -1 {
						barn_error_show_with_line(
							SYNTAX_ERROR, fmt.Sprintf("Unknown return type `%s`", parser.curr_token.value),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else {
						variable_return = return_type
						skip_token(parser, 1)
						if parser.curr_token.kind == OPENBRACE {
							is_ok = true
						} else {
							barn_error_show_with_line(
								SYNTAX_ERROR, "Expected `{` after return type",
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						}
					}
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Expected `{` after function declaration",
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}

				if is_ok == true {
					var function_node NodeAST
					function_node.node_kind = FUNCTION_DECLARATION
					function_node.node_kind_str = "FunctionDeclaration"
					function_node.function_name = function_name
					function_node.function_args = function_args
					function_node.function_return = variable_return
					function_node.function_body = []*NodeAST{}

					append_node(parser, function_node)
					parser.functions = append(parser.functions, &function_node)
					parser.is_function_opened = true
					parser.actual_function = &function_node

					// Set args as local variables
					for i := 0; i < len(function_node.function_args); i++ {
						arg := function_node.function_args[i]
						variable_node := NodeAST{}
						variable_node.node_kind = VARIABLE_DECLARATION
						variable_node.node_kind_str = "VariableDeclaration"
						variable_node.variable_name = arg.name
						variable_node.variable_type = arg.type_arg
						variable_node.variable_value = ""
						variable_node.variable_is_arg = true
						append_node(parser, variable_node)
						parser.local_variables = append(parser.local_variables, &variable_node)
					}
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Something gone wrong while declaring function",
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected `OPENBRACE` `{` after function declaration",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Function is already opened",
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

// Are we actually in function if not then throw out an error
func error_when_we_arent_in_function(parser *Parser) {
	if parser.is_function_opened == false {
		barn_error_show_with_line(
			SYNTAX_ERROR, "This token can be use only in function body",
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

func find_function(parser *Parser, function_name string) *NodeAST {
	for i := 0; i < len(parser.functions); i++ {
		if parser.functions[i].function_name == function_name {
			return parser.functions[i]
		}
	}

	return nil
}

func change_token_to_barn_type(parser *Parser, tk *Token) (BarnTypes, bool) {
	switch tk.kind {
	case STRING:
		return BARN_STR, false
	case INT:
		return BARN_INTREGER, false
	case FLOAT:
		return BARN_FLOAT, false
	case CHAR:
		return BARN_CHAR, false
	case IDENTIFIER:
		if tk.value == "true" || tk.value == "false" {
			return BARN_BOOL, false
		} else {
			find_var := find_variable_both(parser, tk.value)
			if find_var == nil {
				barn_error_show_with_line(
					UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", tk.value),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			} else {
				return find_var.variable_type, true
			}
		}
	}
	return BARN_TYPE_NONE, false
}

// Function for parsing function calls
func parse_function_call(parser *Parser, function_name string) {
	error_when_we_arent_in_function(parser)
	mentioned_function := find_function(parser, function_name)

	if mentioned_function == nil {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("Function with name `%s` can't be found", function_name),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1-len(function_name),
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	} else {
		argument_count := 0
		arguments_to_node := []ArgsFunctionCall{}
		skip_token(parser, 1)
		for parser.curr_token.kind == STRING || parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == IDENTIFIER || parser.curr_token.kind == COMMA || parser.curr_token.kind == CHAR {
			if argument_count == len(mentioned_function.function_args) {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Too many arguments have been passed, expected only %d to call function `%s`", len(mentioned_function.function_args), function_name),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			} else if parser.curr_token.kind == STRING || parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == IDENTIFIER || parser.curr_token.kind == CHAR {
				tk_barn_type, is_var := change_token_to_barn_type(parser, parser.curr_token)
				if tk_barn_type == mentioned_function.function_args[argument_count].type_arg {
					arguments_to_node = append(arguments_to_node, ArgsFunctionCall{
						mentioned_function.function_args[argument_count].name,
						mentioned_function.function_args[argument_count].type_arg,
						parser.curr_token.value,
						is_var})
					argument_count++

					skip_token(parser, 1)
					if parser.curr_token.kind == COMMA {
						// barn_error_show_with_line(
						// 	SYNTAX_ERROR, "Expected use of comma in this place",
						// 	parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						// 	true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						// os.Exit(1)
						skip_token(parser, 1)
					}
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, fmt.Sprintf("Argument type is `%s` not `%s`", mentioned_function.function_args[argument_count].type_arg.as_string(), tk_barn_type.as_string()),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
				// argument := parse_value(parser, mentioned_function.function_args[argument_count].type_arg)
				// fmt.Println(argument)
				// arguments_to_node = append(arguments_to_node, ArgsFunctionCall{
				// 	mentioned_function.function_args[argument_count].name,
				// 	mentioned_function.function_args[argument_count].type_arg,
				// 	parser.curr_token.value})
				// argument_count++
				// if mentioned_function.function_args[argument_count].type_arg == FLOAT || mentioned_function.function_args[argument_count].type_arg == INT {
				// 	skip_token(parser, -1)
				// }
				// skip_token(parser, 1)
				// if parser.curr_token.kind == COMMA {
				// 	skip_token(parser, 1)
				// }
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Unexpected use of comma in this place",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		}
		if argument_count == len(mentioned_function.function_args) {
			if parser.curr_token.kind == CLOSEPARENT {
				function_call_node := NodeAST{}
				function_call_node.node_kind = FUNCTION_CALL
				function_call_node.node_kind_str = "FunctionCall"

				function_call_node.call_args = arguments_to_node
				function_call_node.call_name = mentioned_function.function_name
				function_call_node.call_func = mentioned_function
				append_node(parser, function_call_node)
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "At the end of function call expected `)`",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Too few arguments have been passed, expected %d arguments to call function `%s`", len(mentioned_function.function_args), function_name),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

// Function that helps and parse variable value
func parse_variable_value(parser *Parser, expected_type BarnTypes) (bool, string) {
	skip_token(parser, 1)
	do_we_continue := true
	expect_symbol := false
	expect_number := true
	parents := 0
	to_ret := ""
	first_token_kind := parser.curr_token.kind
	skip_token(parser, 1)
	second_token_kind := parser.curr_token.kind
	skip_token(parser, -1)
	identifier_and_math := first_token_kind == IDENTIFIER && (second_token_kind == PLUS || second_token_kind == MINUS || second_token_kind == MUL || second_token_kind == DIV || second_token_kind == MOD)
	if parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT || identifier_and_math {
		for do_we_continue {
			if (parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT) && expect_number == true {
				to_ret += parser.curr_token.value
				expect_symbol = true
				expect_number = false
				skip_token(parser, 1)
				continue
			} else if parser.curr_token.kind == IDENTIFIER && expect_number == true {
				function_name := parser.curr_token.value
				if find_var := find_variable_both(parser, parser.curr_token.value); find_var != nil {
					if find_var.variable_type == BARN_INTREGER || find_var.variable_type == BARN_FLOAT {
						to_ret += parser.curr_token.value
						expect_symbol = true
						expect_number = false
						skip_token(parser, 1)
						continue
					} else {
						barn_error_show_with_line(
							UNDEFINED_ERROR, fmt.Sprintf("Expected variable with type `int` or `float` not `%s`", find_var.variable_type.as_string()),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				} else {
					skip_token(parser, 1)
					if parser.curr_token.kind == OPENPARENT {
						mentioned_function := find_function(parser, function_name)
						if mentioned_function == nil {
							barn_error_show_with_line(
								UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct function name", parser.curr_token.value),
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						} else {
							argument_count := 0
							arguments_to_node := []ArgsFunctionCall{}
							skip_token(parser, 1)
							for parser.curr_token.kind == STRING || parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == IDENTIFIER || parser.curr_token.kind == COMMA || parser.curr_token.kind == CHAR {
								if argument_count == len(mentioned_function.function_args) {
									barn_error_show_with_line(
										SYNTAX_ERROR, fmt.Sprintf("Too many arguments have been passed, expected only %d to call function `%s`", len(mentioned_function.function_args), function_name),
										parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
										true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
									os.Exit(1)
								} else if parser.curr_token.kind == STRING || parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == IDENTIFIER || parser.curr_token.kind == CHAR {
									tk_barn_type, is_var := change_token_to_barn_type(parser, parser.curr_token)
									if tk_barn_type == mentioned_function.function_args[argument_count].type_arg {
										arguments_to_node = append(arguments_to_node, ArgsFunctionCall{
											mentioned_function.function_args[argument_count].name,
											mentioned_function.function_args[argument_count].type_arg,
											parser.curr_token.value,
											is_var})
										argument_count++

										skip_token(parser, 1)
										if parser.curr_token.kind == COMMA {
											// barn_error_show_with_line(
											// 	SYNTAX_ERROR, "Expected use of comma in this place",
											// 	parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
											// 	true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
											// os.Exit(1)
											skip_token(parser, 1)
										}
									} else {
										barn_error_show_with_line(
											SYNTAX_ERROR, fmt.Sprintf("Argument type is `%s` not `%s`", mentioned_function.function_args[argument_count].type_arg.as_string(), tk_barn_type.as_string()),
											parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
											true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
										os.Exit(1)
									}
									// argument := parse_value(parser, mentioned_function.function_args[argument_count].type_arg)
									// fmt.Println(argument)
									// arguments_to_node = append(arguments_to_node, ArgsFunctionCall{
									// 	mentioned_function.function_args[argument_count].name,
									// 	mentioned_function.function_args[argument_count].type_arg,
									// 	parser.curr_token.value})
									// argument_count++
									// if mentioned_function.function_args[argument_count].type_arg == FLOAT || mentioned_function.function_args[argument_count].type_arg == INT {
									// 	skip_token(parser, -1)
									// }
									// skip_token(parser, 1)
									// if parser.curr_token.kind == COMMA {
									// 	skip_token(parser, 1)
									// }
								} else {
									barn_error_show_with_line(
										SYNTAX_ERROR, "Unexpected use of comma in this place",
										parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
										true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
									os.Exit(1)
								}
							}
							if argument_count == len(mentioned_function.function_args) {
								if parser.curr_token.kind == CLOSEPARENT {
									to_ret += fmt.Sprintf("%s(", function_name)
									if 0 == len(arguments_to_node) {
										to_ret += ");\n"
									} else {
										for k := 0; k < len(arguments_to_node); k++ {
											pass_argument_value := arguments_to_node[k]
											if pass_argument_value.is_var == true {
												to_ret += pass_argument_value.value
											} else {
												if pass_argument_value.type_arg == BARN_STR {
													to_ret += "\""
													to_ret += pass_argument_value.value
													to_ret += "\""
												} else if pass_argument_value.type_arg == BARN_CHAR {
													to_ret += "'"
													to_ret += pass_argument_value.value
													to_ret += "'"
												} else if pass_argument_value.type_arg == BARN_FLOAT {
													to_ret += "(float)"
													to_ret += pass_argument_value.value
												} else {
													to_ret += pass_argument_value.value
												}
											}

											if (k + 1) != len(arguments_to_node) {
												to_ret += ", "
											} else {
												to_ret += ");\n"
											}
										}
									}
								} else {
									barn_error_show_with_line(
										SYNTAX_ERROR, "At the end of function call expected `)`",
										parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
										true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
									os.Exit(1)
								}
							} else {
								barn_error_show_with_line(
									SYNTAX_ERROR, fmt.Sprintf("Too few arguments have been passed, expected %d arguments to call function `%s`", len(mentioned_function.function_args), function_name),
									parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
									true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
								os.Exit(1)
							}
						}
					} else {
						barn_error_show_with_line(
							UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				}
			} else if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && expect_symbol == true {
				to_ret += parser.curr_token.value
				expect_symbol = false
				expect_number = true
				skip_token(parser, 1)
				continue
			} else if parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT {
				if parser.curr_token.kind == OPENPARENT {
					to_ret += parser.curr_token.value
					parents += 1
					expect_number = true
					expect_symbol = false
					skip_token(parser, 1)
					continue
				} else {
					if parents == 0 {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Unexpected use of `)`",
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else {
						to_ret += parser.curr_token.value
						parents -= 1
						expect_number = false
						expect_symbol = true
						skip_token(parser, 1)
						continue
					}
				}
			} else if expect_number == true {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected number",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			} else if expect_symbol == true {
				break
			} else {
				break
			}
		}
		if expected_type == BARN_INTREGER || expected_type == BARN_FLOAT {
			return false, to_ret
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Variable type is `int` or `float` not `%s`", expected_type.as_string()),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else if parser.curr_token.kind == IDENTIFIER {
		if parser.curr_token.value == "true" || parser.curr_token.value == "false" {
			if expected_type == BARN_BOOL {
				return false, parser.curr_token.value
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Variable type is `bool` not `%s`", expected_type.as_string()),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			function_name := parser.curr_token.value
			if find_var := find_variable_both(parser, parser.curr_token.value); find_var != nil {
				if find_var.variable_type == BARN_INTREGER || find_var.variable_type == BARN_FLOAT {
					to_ret += parser.curr_token.value
					expect_symbol = true
					expect_number = false
					skip_token(parser, 1)
				} else {
					barn_error_show_with_line(
						UNDEFINED_ERROR, fmt.Sprintf("Expected variable with type `int` or `float` not `%s`", find_var.variable_type.as_string()),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				skip_token(parser, 1)
				if parser.curr_token.kind == OPENPARENT {
					mentioned_function := find_function(parser, function_name)
					if mentioned_function == nil {
						barn_error_show_with_line(
							UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct function name", parser.curr_token.value),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else {
						argument_count := 0
						arguments_to_node := []ArgsFunctionCall{}
						skip_token(parser, 1)
						for parser.curr_token.kind == STRING || parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == IDENTIFIER || parser.curr_token.kind == COMMA || parser.curr_token.kind == CHAR {
							if argument_count == len(mentioned_function.function_args) {
								barn_error_show_with_line(
									SYNTAX_ERROR, fmt.Sprintf("Too many arguments have been passed, expected only %d to call function `%s`", len(mentioned_function.function_args), function_name),
									parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
									true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
								os.Exit(1)
							} else if parser.curr_token.kind == STRING || parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT || parser.curr_token.kind == IDENTIFIER || parser.curr_token.kind == CHAR {
								tk_barn_type, is_var := change_token_to_barn_type(parser, parser.curr_token)
								if tk_barn_type == mentioned_function.function_args[argument_count].type_arg {
									arguments_to_node = append(arguments_to_node, ArgsFunctionCall{
										mentioned_function.function_args[argument_count].name,
										mentioned_function.function_args[argument_count].type_arg,
										parser.curr_token.value,
										is_var})
									argument_count++

									skip_token(parser, 1)
									if parser.curr_token.kind == COMMA {
										// barn_error_show_with_line(
										// 	SYNTAX_ERROR, "Expected use of comma in this place",
										// 	parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
										// 	true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
										// os.Exit(1)
										skip_token(parser, 1)
									}
								} else {
									barn_error_show_with_line(
										SYNTAX_ERROR, fmt.Sprintf("Argument type is `%s` not `%s`", mentioned_function.function_args[argument_count].type_arg.as_string(), tk_barn_type.as_string()),
										parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
										true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
									os.Exit(1)
								}
								// argument := parse_value(parser, mentioned_function.function_args[argument_count].type_arg)
								// fmt.Println(argument)
								// arguments_to_node = append(arguments_to_node, ArgsFunctionCall{
								// 	mentioned_function.function_args[argument_count].name,
								// 	mentioned_function.function_args[argument_count].type_arg,
								// 	parser.curr_token.value})
								// argument_count++
								// if mentioned_function.function_args[argument_count].type_arg == FLOAT || mentioned_function.function_args[argument_count].type_arg == INT {
								// 	skip_token(parser, -1)
								// }
								// skip_token(parser, 1)
								// if parser.curr_token.kind == COMMA {
								// 	skip_token(parser, 1)
								// }
							} else {
								barn_error_show_with_line(
									SYNTAX_ERROR, "Unexpected use of comma in this place",
									parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
									true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
								os.Exit(1)
							}
						}
						if argument_count == len(mentioned_function.function_args) {
							if parser.curr_token.kind == CLOSEPARENT {
								to_ret += fmt.Sprintf("	%s(", function_name)
								if 0 == len(arguments_to_node) {
									to_ret += ");\n"
								} else {
									for k := 0; k < len(arguments_to_node); k++ {
										pass_argument_value := arguments_to_node[k]
										if pass_argument_value.is_var == true {
											to_ret += pass_argument_value.value
										} else {
											if pass_argument_value.type_arg == BARN_STR {
												to_ret += "\""
												to_ret += pass_argument_value.value
												to_ret += "\""
											} else if pass_argument_value.type_arg == BARN_CHAR {
												to_ret += "'"
												to_ret += pass_argument_value.value
												to_ret += "'"
											} else if pass_argument_value.type_arg == BARN_FLOAT {
												to_ret += "(float)"
												to_ret += pass_argument_value.value
											} else {
												to_ret += pass_argument_value.value
											}
										}

										if (k + 1) != len(arguments_to_node) {
											to_ret += ", "
										} else {
											to_ret += ");\n"
										}
									}
								}
								// skip_token(parser, 1)
							} else {
								barn_error_show_with_line(
									SYNTAX_ERROR, "At the end of function call expected `)`",
									parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
									true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
								os.Exit(1)
							}
						} else {
							barn_error_show_with_line(
								SYNTAX_ERROR, fmt.Sprintf("Too few arguments have been passed, expected %d arguments to call function `%s`", len(mentioned_function.function_args), function_name),
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						}

						if expected_type == mentioned_function.function_return {
							return true, to_ret
						} else {
							barn_error_show_with_line(
								SYNTAX_ERROR, fmt.Sprintf("Function return type is `%s` not `%s`", mentioned_function.function_return.as_string(), expected_type.as_string()),
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						}
					}
				} else {
					barn_error_show_with_line(
						UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			}

			// to_ret_2 := parser.curr_token.value
			// skip_token(parser, 1)
			// if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && (expected_type == BARN_INTREGER || expected_type == BARN_FLOAT) {
			// 	to_ret += to_ret_2
			// 	expect_number = false
			// 	expect_symbol = true
			// 	for do_we_continue {
			// 		if (parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT) && expect_number == true {
			// 			to_ret += parser.curr_token.value
			// 			expect_symbol = true
			// 			expect_number = false
			// 			skip_token(parser, 1)
			// 			continue
			// 		} else if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && expect_symbol == true {
			// 			to_ret += parser.curr_token.value
			// 			expect_symbol = false
			// 			expect_number = true
			// 			skip_token(parser, 1)
			// 			continue
			// 		} else if parser.curr_token.kind == IDENTIFIER && expect_number == true {
			// 			if find_var := find_variable_both(parser, parser.curr_token.value); find_var != nil {
			// 				if find_var.variable_type == BARN_INTREGER || find_var.variable_type == BARN_FLOAT {
			// 					to_ret += parser.curr_token.value
			// 					expect_symbol = true
			// 					expect_number = false
			// 					skip_token(parser, 1)
			// 					continue
			// 				} else {
			// 					barn_error_show_with_line(
			// 						UNDEFINED_ERROR, fmt.Sprintf("Expected variable with type `int` or `float` not `%s`", find_var.variable_type.as_string()),
			// 						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 					os.Exit(1)
			// 				}
			// 			} else {
			// 				barn_error_show_with_line(
			// 					UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
			// 					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 				os.Exit(1)
			// 			}
			// 		} else if parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT {
			// 			if parser.curr_token.kind == OPENPARENT {
			// 				to_ret += parser.curr_token.value
			// 				parents += 1
			// 				expect_number = true
			// 				expect_symbol = false
			// 				skip_token(parser, 1)
			// 				continue
			// 			} else {
			// 				if parents == 0 {
			// 					barn_error_show_with_line(
			// 						SYNTAX_ERROR, "Unexpected use of `)`",
			// 						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 					os.Exit(1)
			// 				} else {
			// 					to_ret += parser.curr_token.value
			// 					parents -= 1
			// 					expect_number = false
			// 					expect_symbol = true
			// 					skip_token(parser, 1)
			// 					continue
			// 				}
			// 			}
			// 		} else if expect_number == true {
			// 			barn_error_show_with_line(
			// 				SYNTAX_ERROR, "Expected number",
			// 				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 			os.Exit(1)
			// 		} else if expect_symbol == true {
			// 			break
			// 		} else {
			// 			break
			// 		}
			// 	}
			// 	if expected_type == BARN_INTREGER || expected_type == BARN_FLOAT {
			// 		return to_ret
			// 	} else {
			// 		barn_error_show_with_line(
			// 			SYNTAX_ERROR, fmt.Sprintf("Variable type is `int` or `float` not `%s`", expected_type.as_string()),
			// 			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 		os.Exit(1)
			// 	}
			// } else {
			// skip_token(parser, -1)
			// find_var := find_variable_both(parser, parser.curr_token.value)
			// if find_var == nil {
			// 	barn_error_show_with_line(
			// 		UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
			// 		parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 		true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 	os.Exit(1)
			// } else {
			// 	if expected_type == find_var.variable_type {
			// 		skip_token(parser, 1)
			// 		return to_ret_2
			// 	} else {
			// 		barn_error_show_with_line(
			// 			SYNTAX_ERROR, fmt.Sprintf("RHS is type `%s` not like variable type with type `%s`", find_variable_both(parser, parser.curr_token.value).variable_type.as_string(), expected_type.as_string()),
			// 			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			// 			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			// 		os.Exit(1)
			// 	}
			// }
			// }
		}
	} else if parser.curr_token.kind == STRING {
		if expected_type == BARN_STR {
			return false, parser.curr_token.value
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Variable type is `string` not `%s`", expected_type.as_string()),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else if parser.curr_token.kind == CHAR {
		if expected_type == BARN_CHAR {
			return false, parser.curr_token.value
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Variable type is `char` not `%s`", expected_type.as_string()),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, fmt.Sprintf("Unexpected use of `%s`", kind_to_str(parser.curr_token.kind)),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
		return false, "none"
	}
	return false, "none"
}

func is_variable_defined(parser *Parser, name string) bool {
	for i := 0; i < len(parser.local_variables); i++ {
		if parser.local_variables[i].variable_name == name {
			return true
		}
	}

	return false
}

func find_variable(parser *Parser, name string) *NodeAST {
	for i := 0; i < len(parser.local_variables); i++ {
		if parser.local_variables[i].variable_name == name {
			return parser.local_variables[i]
		}
	}

	return nil
}

func is_variable_defined_global(parser *Parser, name string) bool {
	for i := 0; i < len(parser.global_variables); i++ {
		if parser.global_variables[i].variable_name == name {
			return true
		}
	}

	return false
}

func find_variable_global(parser *Parser, name string) *NodeAST {
	for i := 0; i < len(parser.global_variables); i++ {
		if parser.global_variables[i].variable_name == name {
			return parser.global_variables[i]
		}
	}

	return nil
}

func find_variable_both(parser *Parser, name string) *NodeAST {
	for i := 0; i < len(parser.global_variables); i++ {
		if parser.global_variables[i].variable_name == name {
			return parser.global_variables[i]
		}
	}

	for i := 0; i < len(parser.local_variables); i++ {
		if parser.local_variables[i].variable_name == name {
			return parser.local_variables[i]
		}
	}

	return nil
}

func reset_local_variables(parser *Parser) {
	parser.local_variables = []*NodeAST{}
}

// Function that parse `let` keyword for declaring variables
func parse_let(parser *Parser) {
	// error_when_we_arent_in_function(parser)
	if parser.is_function_opened == true {
		if is_next_token_kind_safe(parser, IDENTIFIER) {
			variable_name := parser.curr_token.value

			if is_variable_defined(parser, variable_name) == true || is_variable_defined_global(parser, variable_name) == true {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Variable with name `%s` is already defined", variable_name),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}

			if is_next_token_kind_safe(parser, COLON) {
				if is_next_token_kind_safe(parser, IDENTIFIER) {
					variable_type := is_token_represent_type(parser.curr_token.value)
					if variable_type == BARN_TYPE_NONE {
						barn_error_show_with_line(
							SYNTAX_ERROR, fmt.Sprintf("Unknown variable type `%s`", parser.curr_token.value),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else {
						if is_next_token_kind_safe(parser, ASN) {
							is_function_call_value, variable_value := parse_variable_value(parser, variable_type)

							// fmt.Printf("Variable name: `%s`, Variable type: `%s`, Variable value: `%s`\n", variable_name, variable_type.as_string(), variable_value)

							variable_node := NodeAST{}
							variable_node.node_kind = VARIABLE_DECLARATION
							variable_node.node_kind_str = "VariableDeclaration"
							variable_node.variable_name = variable_name
							variable_node.variable_type = variable_type
							variable_node.variable_value = variable_value
							variable_node.variable_fn_call_value = is_function_call_value
							variable_node.variable_is_arg = false
							append_node(parser, variable_node)
							parser.local_variables = append(parser.local_variables, &variable_node)

							if (variable_type == BARN_INTREGER || variable_type == BARN_FLOAT) && is_function_call_value == false {
								skip_token(parser, -1)
							} else if is_function_call_value {
								skip_token(parser, 0)
							}
						} else {
							barn_error_show_with_line(
								SYNTAX_ERROR, "Expected `=` after variable type",
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						}
					}
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Expected identifier that represents variable type",
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected `:` after variable name",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected identifier that represents variable name",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		if is_next_token_kind_safe(parser, IDENTIFIER) {
			variable_name := parser.curr_token.value

			if is_variable_defined_global(parser, variable_name) == true {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Variable with name `%s` is already defined", variable_name),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}

			if is_next_token_kind_safe(parser, COLON) {
				if is_next_token_kind_safe(parser, IDENTIFIER) {
					variable_type := is_token_represent_type(parser.curr_token.value)
					if variable_type == BARN_TYPE_NONE {
						barn_error_show_with_line(
							SYNTAX_ERROR, fmt.Sprintf("Unknown variable type `%s`", parser.curr_token.value),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					} else {
						if is_next_token_kind_safe(parser, ASN) {
							is_variable_value_fn_call, variable_value := parse_variable_value(parser, variable_type)

							// fmt.Printf("Variable name: `%s`, Variable type: `%s`, Variable value: `%s`\n", variable_name, variable_type.as_string(), variable_value)

							variable_node := NodeAST{}
							variable_node.node_kind = VARIABLE_DECLARATION
							variable_node.node_kind_str = "VariableDeclaration"
							variable_node.variable_name = variable_name
							variable_node.variable_type = variable_type
							variable_node.variable_value = variable_value
							variable_node.variable_fn_call_value = is_variable_value_fn_call
							append_node(parser, variable_node)
							parser.global_variables = append(parser.global_variables, &variable_node)

							if variable_type == BARN_INTREGER || variable_type == BARN_FLOAT {
								skip_token(parser, -1)
							}
						} else {
							barn_error_show_with_line(
								SYNTAX_ERROR, "Expected `=` after variable type",
								parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
								true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
							os.Exit(1)
						}
					}
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Expected identifier that represents variable type",
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected `:` after variable name",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected identifier that represents variable name",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

// Function that parse `@import_c` keyword
// ----------------------------------------------------
// @import_c "stdio.h"
// ----------------------------------------------------
func parse_import_c(parser *Parser) {
	// if parser.is_function_opened == true {
	// 	barn_error_show_with_line(
	// 		SYNTAX_ERROR, "`@import_c` must be outside of function",
	// 		parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
	// 		true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
	// 	os.Exit(1)
	// } else {
	is_next_token_kind(parser, STRING)
	if len(parser.curr_token.value) == 0 {
		barn_error_show_with_line(
			SYNTAX_ERROR, "C header is too short",
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		return
	}
	node := NodeAST{}
	node.node_kind = IMPORT_C
	node.node_kind_str = "ImportC"
	node.import_c_header = parser.curr_token.value
	append_node(parser, node)
	// }
}

// Function for parsing variable assignments
// ----------------------------------------------------
// variable1 = <expression of type that is variable>
// ----------------------------------------------------
func parse_variable_asn(parser *Parser, variable_name string) {
	variable := find_variable_both(parser, variable_name)

	if variable == nil {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` variable is not defined", variable_name),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	} else {
		value := parse_value(parser, variable.variable_type)
		if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
			skip_token(parser, -1)
		}
		node := NodeAST{}
		node.node_kind = VARIABLE_ASSIGNMENT
		node.node_kind_str = "VariableAssignmemt"
		node.variable_assignment_name = variable_name
		node.variable_assignment_value = value
		append_node(parser, node)
	}
}

// Function for parsing variable plus assignments
// ----------------------------------------------------
// variable1 += <expression of type that is variable>
// ----------------------------------------------------
func parse_variable_plus_asn(parser *Parser, variable_name string) {
	variable := find_variable_both(parser, variable_name)

	if variable == nil {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` variable is not defined", variable_name),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	} else {
		if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
			value := parse_value(parser, variable.variable_type)
			if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
				skip_token(parser, -1)
			}
			node := NodeAST{}
			node.node_kind = VARIABLE_PLUS_ASSIGNMENT
			node.node_kind_str = "VariablePlusAssignmemt"
			node.variable_plus_assignment_name = variable_name
			node.variable_plus_assignment_value = value
			append_node(parser, node)
		} else {
			barn_error_show_with_line(
				UNDEFINED_ERROR, "`+=` can be only used when variable type is `int` or `float`",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

// Function for parsing variable minus assignments
// ----------------------------------------------------
// variable1 -= <expression of type that is variable>
// ----------------------------------------------------
func parse_variable_minus_asn(parser *Parser, variable_name string) {
	variable := find_variable_both(parser, variable_name)

	if variable == nil {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` variable is not defined", variable_name),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	} else {
		if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
			value := parse_value(parser, variable.variable_type)
			if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
				skip_token(parser, -1)
			}
			node := NodeAST{}
			node.node_kind = VARIABLE_MINUS_ASSIGNMENT
			node.node_kind_str = "VariableMinusAssignmemt"
			node.variable_minus_assignment_name = variable_name
			node.variable_minus_assignment_value = value
			append_node(parser, node)
		} else {
			barn_error_show_with_line(
				UNDEFINED_ERROR, "`-=` can be only used when variable type is `int` or `float`",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

// Function for parsing variable mul assignments
// ----------------------------------------------------
// variable1 *= <expression of type that is variable>
// ----------------------------------------------------
func parse_variable_mul_asn(parser *Parser, variable_name string) {
	variable := find_variable_both(parser, variable_name)

	if variable == nil {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` variable is not defined", variable_name),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	} else {
		if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
			value := parse_value(parser, variable.variable_type)
			if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
				skip_token(parser, -1)
			}
			node := NodeAST{}
			node.node_kind = VARIABLE_MUL_ASSIGNMENT
			node.node_kind_str = "VariableMulAssignmemt"
			node.variable_mul_assignment_name = variable_name
			node.variable_mul_assignment_value = value
			append_node(parser, node)
		} else {
			barn_error_show_with_line(
				UNDEFINED_ERROR, "`*=` can be only used when variable type is `int` or `float`",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

// Function for parsing variable div assignments
// ----------------------------------------------------
// variable1 /= <expression of type that is variable>
// ----------------------------------------------------
func parse_variable_div_asn(parser *Parser, variable_name string) {
	variable := find_variable_both(parser, variable_name)

	if variable == nil {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` variable is not defined", variable_name),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	} else {
		if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
			value := parse_value(parser, variable.variable_type)
			if variable.variable_type == BARN_INTREGER || variable.variable_type == BARN_FLOAT {
				skip_token(parser, -1)
			}
			node := NodeAST{}
			node.node_kind = VARIABLE_DIV_ASSIGNMENT
			node.node_kind_str = "VariableDivAssignmemt"
			node.variable_div_assignment_name = variable_name
			node.variable_div_assignment_value = value
			append_node(parser, node)
		} else {
			barn_error_show_with_line(
				UNDEFINED_ERROR, "`/=` can be only used when variable type is `int` or `float`",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

// Function that help to parse return keyword
func parse_return(parser *Parser) {
	if parser.is_function_opened {
		function := find_function(parser, parser.actual_function.function_name)
		return_value := parse_value(parser, function.function_return)
		if function.function_return == BARN_INTREGER || function.function_return == BARN_FLOAT {
			skip_token(parser, -1)
		}
		node := NodeAST{}
		node.node_kind = FUNCTION_RETURN
		node.node_kind_str = "FunctionReturn"
		node.function_return_value = return_value
		append_node(parser, node)
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "To return function value you need to first open it",
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

// Function that parse condition statements
func parse_condition_statements(parser *Parser, end_kind int) string {
	skip_token(parser, 1)
	var last_type_lhs BarnTypes = BARN_TYPE_NONE
	var last_symbol int = NONE
	var last_symbol_str string = ""
	do_we_continue := true
	expected_value := true
	expected_symbol := false
	parents := 0
	to_ret := ""
	for do_we_continue {
		if parser.curr_token.kind == end_kind {
			if to_ret == "" {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected condition statement between `|`", parser.curr_token.filename,
					parser.curr_token.row, parser.curr_token.col-1, true,
					parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			} else {
				if expected_symbol && !expected_value {
					return to_ret
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Expected symbol after number not close", parser.curr_token.filename,
						parser.curr_token.row, parser.curr_token.col-1, true,
						parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			}
		} else if (parser.curr_token.kind == INT || parser.curr_token.kind == FLOAT) && expected_value {
			if last_type_lhs == BARN_TYPE_NONE || (last_type_lhs == BARN_INTREGER || last_type_lhs == BARN_FLOAT) {
				to_ret += parser.curr_token.value
				expected_symbol = true
				expected_value = false
				skip_token(parser, 1)
				if last_type_lhs == BARN_TYPE_NONE {
					last_type_lhs = BARN_INTREGER
				}
				continue
			} else {
				kind, _ := change_token_to_barn_type(parser, parser.curr_token)
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Expected type `%s` not `%s`", last_type_lhs.as_string(), kind.as_string()),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else if (parser.curr_token.kind == IDENTIFIER) && expected_value {
			if find_var := find_variable_both(parser, parser.curr_token.value); find_var != nil {
				if last_type_lhs == BARN_TYPE_NONE || last_type_lhs == find_var.variable_type {
					to_ret += parser.curr_token.value
					expected_symbol = true
					expected_value = false
					skip_token(parser, 1)
					if last_type_lhs == BARN_TYPE_NONE {
						last_type_lhs = find_var.variable_type
					}
					continue
				} else {
					barn_error_show_with_line(
						UNDEFINED_ERROR, fmt.Sprintf("Expected variable with type `%s` not `%s`", last_type_lhs.as_string(), find_var.variable_type.as_string()),
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else if parser.curr_token.value == "true" || parser.curr_token.value == "false" {
				to_ret += parser.curr_token.value
				expected_symbol = true
				expected_value = false
				skip_token(parser, 1)
				continue
			} else {
				barn_error_show_with_line(
					UNDEFINED_ERROR, fmt.Sprintf("Variable with name `%s` is not defined", parser.curr_token.value),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else if (parser.curr_token.kind == STRING) && expected_value {
			if last_type_lhs == BARN_TYPE_NONE || last_type_lhs == BARN_STR {
				if last_type_lhs == BARN_STR {
					if last_symbol == EQ || last_symbol == NEQ {
						to_ret += "\"" + parser.curr_token.value + "\""
					} else {
						barn_error_show_with_line(
							SYNTAX_ERROR, fmt.Sprintf("Symbol `%s` can be used with string comparation", last_symbol_str),
							parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
							true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}

				} else {
					to_ret += fmt.Sprintf("barn_string_compare(\"%s\"", parser.curr_token.value)
				}
				expected_symbol = true
				expected_value = false
				skip_token(parser, 1)
				if last_type_lhs == BARN_TYPE_NONE {
					last_type_lhs = BARN_STR
				}
				continue
			} else {
				kind, _ := change_token_to_barn_type(parser, parser.curr_token)
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Expected type `%s` not `%s`", last_type_lhs.as_string(), kind.as_string()),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else if (parser.curr_token.kind == GT || parser.curr_token.kind == GTE || parser.curr_token.kind == LT || parser.curr_token.kind == LTE || parser.curr_token.kind == EQ || parser.curr_token.kind == NEQ || parser.curr_token.kind == OROR || parser.curr_token.kind == ANDAND) && expected_symbol {
			to_ret += parser.curr_token.value
			expected_symbol = false
			expected_value = true
			last_symbol = parser.curr_token.kind
			last_symbol_str = parser.curr_token.value
			skip_token(parser, 1)
			continue
		} else if (parser.curr_token.kind == PLUS || parser.curr_token.kind == MINUS || parser.curr_token.kind == MUL || parser.curr_token.kind == DIV || parser.curr_token.kind == MOD) && expected_symbol == true {
			if last_type_lhs == BARN_STR {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Can't use `%s` with `%s` type", parser.curr_token.value, last_type_lhs.as_string()),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			} else {
				to_ret += parser.curr_token.value
			}
			expected_symbol = false
			expected_value = true
			last_symbol = parser.curr_token.kind
			last_symbol_str = parser.curr_token.value
			skip_token(parser, 1)
			continue
		} else if parser.curr_token.kind == OPENPARENT || parser.curr_token.kind == CLOSEPARENT {
			if parser.curr_token.kind == OPENPARENT {
				to_ret += parser.curr_token.value
				parents += 1
				expected_value = true
				expected_symbol = false
				// last_symbol = parser.curr_token.kind
				// last_symbol_str = parser.curr_token.value
				skip_token(parser, 1)
				continue
			} else {
				if parents == 0 {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Unexpected use of `)`",
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				} else {
					to_ret += parser.curr_token.value
					parents -= 1
					expected_value = false
					expected_symbol = true
					last_symbol = parser.curr_token.kind
					skip_token(parser, 1)
					continue
				}
			}
		} else if expected_value == true {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected value",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		} else if expected_symbol == true {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected `|` to close if statement",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Unexpected use of token `%s`", parser.curr_token.value), parser.curr_token.filename,
				parser.curr_token.row, parser.curr_token.col-len(parser.curr_token.value), true,
				parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
	return ""
}

// Function that parse if statements
func parse_if(parser *Parser) {
	error_when_we_arent_in_function(parser)
	skip_token(parser, 1)
	if parser.curr_token.kind == CONDITIONBLOCK {
		condition_statement := parse_condition_statements(parser, CONDITIONBLOCK)

		if_node := NodeAST{}
		if_node.node_kind = IF_STATEMENT
		if_node.node_kind_str = "IfStatement"
		if_node.if_condition = condition_statement

		append_node_ptr(parser, &if_node)
		skip_token(parser, 1)
		if parser.curr_token.kind == OPENBRACE {
			parser.statement_open += 1
			parser.last_statement_opened = "If"
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected `{` to open if body",
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, fmt.Sprintf("Expected `|` to open if condition not `%s`", kind_to_str(parser.curr_token.kind)), parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

// Function that parse else keyword
func parse_else(parser *Parser) {
	error_when_we_arent_in_function(parser)
	function := parser.nodes[len(parser.nodes)-1] // it should be function node
	if len(function.function_body) != 0 {
		if function.function_body[len(function.function_body)-1].node_kind == END_STATEMENT &&
			(function.function_body[len(function.function_body)-1].end_statement_kind == IF_STATEMENT_END ||
				function.function_body[len(function.function_body)-1].end_statement_kind == ELIF_STATEMENT_END) {
			else_node := NodeAST{}
			else_node.node_kind = ELSE_STATEMENT
			else_node.node_kind_str = "ElseStatement"

			append_node_ptr(parser, &else_node)
			skip_token(parser, 1)
			if parser.curr_token.kind == OPENBRACE {
				parser.statement_open += 1
				parser.last_statement_opened = "Else"
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected `{` to open else body",
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected if or elif statement before else", parser.curr_token.filename,
				parser.curr_token.row, parser.curr_token.col-1, true,
				parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Expected if or elif statement before else", parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

// Function that parse elif keyword
func parse_elif(parser *Parser) {
	error_when_we_arent_in_function(parser)
	skip_token(parser, 1)
	if parser.curr_token.kind == CONDITIONBLOCK {
		condition_statement := parse_condition_statements(parser, CONDITIONBLOCK)

		function := parser.nodes[len(parser.nodes)-1] // it should be function node
		if len(function.function_body) != 0 {
			if function.function_body[len(function.function_body)-1].node_kind == END_STATEMENT &&
				(function.function_body[len(function.function_body)-1].end_statement_kind == IF_STATEMENT_END ||
					function.function_body[len(function.function_body)-1].end_statement_kind == ELIF_STATEMENT_END) {
				else_node := NodeAST{}
				else_node.node_kind = ELSE_IF_STATEMENT
				else_node.node_kind_str = "ElseIfStatement"
				else_node.else_if_condition = condition_statement

				append_node_ptr(parser, &else_node)
				skip_token(parser, 1)
				if parser.curr_token.kind == OPENBRACE {
					parser.statement_open += 1
					parser.last_statement_opened = "Elif"
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "Expected `{` to open elif body",
						parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
						true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected if or elif statement before else", parser.curr_token.filename,
					parser.curr_token.row, parser.curr_token.col-1, true,
					parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected if or elif statement before else", parser.curr_token.filename,
				parser.curr_token.row, parser.curr_token.col-1, true,
				parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, fmt.Sprintf("Expected `|` to open if condition not `%s`", kind_to_str(parser.curr_token.kind)), parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

func parse_import(parser *Parser) {
	skip_token(parser, 1)
}

// Function that parse while keyword
func parse_while(parser *Parser) {
	error_when_we_arent_in_function(parser)
	skip_token(parser, 1)
	if parser.curr_token.kind == CONDITIONBLOCK {
		condition := parse_condition_statements(parser, CONDITIONBLOCK)

		while_node := NodeAST{}
		while_node.node_kind = WHILE_STATEMENT
		while_node.node_kind_str = "WhileStatement"
		while_node.while_condition = condition
		append_node(parser, while_node)

		skip_token(parser, 1)
		if parser.curr_token.kind == OPENBRACE {
			parser.statement_open++
			parser.is_while_statement_opened++
			parser.last_statement_opened = "While"
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected `{` after while condition", parser.curr_token.filename,
				parser.curr_token.row, parser.curr_token.col-1, true,
				parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Expected `|` after while keyword", parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
	}
}

func parse_continue(parser *Parser) {
	error_when_we_arent_in_function(parser)
	if parser.is_while_statement_opened >= 1 {
		continue_node := NodeAST{}
		continue_node.node_kind = CONTINUE_STATEMENT
		continue_node.node_kind_str = "ContinueStatement"
		append_node(parser, continue_node)
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Continue statement can be only used in while statement", parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

func parse_break(parser *Parser) {
	error_when_we_arent_in_function(parser)
	if parser.is_while_statement_opened >= 1 {
		break_node := NodeAST{}
		break_node.node_kind = BREAK_STATEMENT
		break_node.node_kind_str = "BreakStatement"
		append_node(parser, break_node)
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Break statement can be only used in while statement", parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

func parse_for(parser *Parser) {
	error_when_we_arent_in_function(parser)
	skip_token(parser, 1)

	if parser.curr_token.kind == IDENTIFIER && parser.curr_token.value == "let" {
		parse_let(parser)
		actual_function := parser.nodes[len(parser.nodes)-1]
		variable_declaration_node := actual_function.function_body[len(actual_function.function_body)-1]
		actual_function.function_body = actual_function.function_body[:len(actual_function.function_body)-1]

		skip_token(parser, 1)
		if parser.curr_token.kind == SEMICOL {
			condition := parse_condition_statements(parser, SEMICOL)

			if parser.curr_token.kind == SEMICOL {
				skip_token(parser, 1)
				if parser.curr_token.value == variable_declaration_node.variable_name {
					parse_identifier(parser)
					actual_function := parser.nodes[len(parser.nodes)-1]
					variable_modification_node := actual_function.function_body[len(actual_function.function_body)-1]
					actual_function.function_body = actual_function.function_body[:len(actual_function.function_body)-1]

					for_node := NodeAST{}
					for_node.node_kind = FOR_STATEMENT
					for_node.node_kind_str = "ForStatement"
					for_node.for_condition = condition
					for_node.for_var_declaration = variable_declaration_node
					for_node.for_var_operation = variable_modification_node
					append_node(parser, for_node)

					skip_token(parser, 1)
					if parser.curr_token.kind == OPENBRACE {
						parser.statement_open++
						parser.is_for_statement_opened++
						parser.last_statement_opened = "For"
					} else {
						barn_error_show_with_line(
							SYNTAX_ERROR, "Expected `{` after for statement", parser.curr_token.filename,
							parser.curr_token.row, parser.curr_token.col-1, true,
							parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
						os.Exit(1)
					}
				} else {
					barn_error_show_with_line(
						SYNTAX_ERROR, "You need to modify variable that you declared in for statement", parser.curr_token.filename,
						parser.curr_token.row, parser.curr_token.col-1, true,
						parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
					os.Exit(1)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, "Expected `;` after for condition", parser.curr_token.filename,
					parser.curr_token.row, parser.curr_token.col-1, true,
					parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, "Expected `;` after variable declaration", parser.curr_token.filename,
				parser.curr_token.row, parser.curr_token.col-1, true,
				parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	} else {
		barn_error_show_with_line(
			SYNTAX_ERROR, "Expected variable declaration expression", parser.curr_token.filename,
			parser.curr_token.row, parser.curr_token.col-1, true,
			parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

func parse_enum(parser *Parser) {
	// TODO: implement enums
}

func parse_incrementation(parser *Parser, variable_name string) {
	error_when_we_arent_in_function(parser)
	if variable := find_variable_both(parser, variable_name); variable != nil {
		incrementation_node := NodeAST{}
		incrementation_node.node_kind = VARIABLE_INCREMENTATION
		incrementation_node.node_kind_str = "VariableIncrementation"
		incrementation_node.variable_name = variable_name
		append_node(parser, incrementation_node)
	} else {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

func parse_decrementation(parser *Parser, variable_name string) {
	error_when_we_arent_in_function(parser)
	if variable := find_variable_both(parser, variable_name); variable != nil {
		decrementation_node := NodeAST{}
		decrementation_node.node_kind = VARIABLE_DECREMENTATION
		decrementation_node.node_kind_str = "VariableDecrementation"
		decrementation_node.variable_name = variable_name
		append_node(parser, decrementation_node)
	} else {
		barn_error_show_with_line(
			UNDEFINED_ERROR, fmt.Sprintf("`%s` is undefined, expected correct variable name", parser.curr_token.value),
			parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
			true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
		os.Exit(1)
	}
}

// Function that helps to parse successfully
// identifier token
func parse_identifier(parser *Parser) {
	id := parser.curr_token.value
	if is_keyword(parser.curr_token.value) {
		switch parser.curr_token.value {
		case "fun":
			parse_function_declaration(parser)
		case "@import_c":
			parse_import_c(parser)
		case "@import":
			parse_import(parser)
		case "let":
			parse_let(parser)
		case "return":
			parse_return(parser)
		case "if":
			parse_if(parser)
		case "else":
			parse_else(parser)
		case "elif":
			parse_elif(parser)
		case "while":
			parse_while(parser)
		case "continue":
			parse_continue(parser)
		case "break":
			parse_break(parser)
		case "for":
			parse_for(parser)
		case "enum":
			parse_enum(parser)
		}
	} else {
		if parser.is_function_opened == true {
			skip_token(parser, 1)
			if parser.curr_token.kind == OPENPARENT {
				// Function calls
				parse_function_call(parser, id)
			} else if parser.curr_token.kind == ASN {
				// Variable assigment
				parse_variable_asn(parser, id)
			} else if parser.curr_token.kind == PLUSASN {
				// Variable plus assigment
				parse_variable_plus_asn(parser, id)
			} else if parser.curr_token.kind == MINUSASN {
				// Variable minus assigment
				parse_variable_minus_asn(parser, id)
			} else if parser.curr_token.kind == MULASN {
				// Variable mul assigment
				parse_variable_mul_asn(parser, id)
			} else if parser.curr_token.kind == DIVASN {
				// Variable div assigment
				parse_variable_div_asn(parser, id)
			} else if parser.curr_token.kind == INCREMENTATION {
				// Variable div assigment
				parse_incrementation(parser, id)
			} else if parser.curr_token.kind == DECREMENTATION {
				// Variable div assigment
				parse_decrementation(parser, id)
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Unexpected use of token `%s` after `IDENTIFIER`", parser.curr_token.value),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("This `IDENTIFIER` can be used only in function", parser.curr_token.value),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}
}

func init_functions_lib(parser *Parser) {
	printf__node__ := NodeAST{}

	printf__node__args := []ArgsFunctionDeclaration{}
	printf__node__args = append(printf__node__args, ArgsFunctionDeclaration{"__str", BARN_STR})

	printf__node__.function_args = printf__node__args
	printf__node__.function_body = nil
	printf__node__.function_name = "printf"
	printf__node__.function_return = BARN_TYPE_NONE

	parser.functions = append(parser.functions, &printf__node__)

	exit__node__ := NodeAST{}

	exit__node__args := []ArgsFunctionDeclaration{}
	exit__node__args = append(exit__node__args, ArgsFunctionDeclaration{"__num", BARN_INTREGER})

	exit__node__.function_args = exit__node__args
	exit__node__.function_body = nil
	exit__node__.function_name = "exit"
	exit__node__.function_return = BARN_TYPE_NONE

	parser.functions = append(parser.functions, &exit__node__)

	putchar_node__ := NodeAST{}

	putchar_node__args := []ArgsFunctionDeclaration{}
	putchar_node__args = append(putchar_node__args, ArgsFunctionDeclaration{"__c", BARN_CHAR})

	putchar_node__.function_args = putchar_node__args
	putchar_node__.function_body = nil
	putchar_node__.function_name = "putchar"
	putchar_node__.function_return = BARN_TYPE_NONE

	parser.functions = append(parser.functions, &putchar_node__)

	puts__node__ := NodeAST{}

	puts__node__.function_args = printf__node__args
	puts__node__.function_body = nil
	puts__node__.function_name = "puts"
	puts__node__.function_return = BARN_TYPE_NONE

	parser.functions = append(parser.functions, &puts__node__)

	__code__node__ := NodeAST{}

	__code__node__args := []ArgsFunctionDeclaration{}
	__code__node__args = append(__code__node__args, ArgsFunctionDeclaration{"__str", BARN_STR})

	__code__node__.function_args = __code__node__args
	__code__node__.function_body = nil
	__code__node__.function_name = "__code__"
	__code__node__.function_return = BARN_TYPE_NONE

	parser.functions = append(parser.functions, &__code__node__)
}

func parser_start(lex *Lexer) *Parser {
	var parser Parser
	parser.lex = lex
	init_functions_lib(&parser)
	for parser.index = 0; parser.index < len(lex.tokens); parser.index++ {
		skip_token(&parser, 0)

		if parser.curr_token.kind == IDENTIFIER {
			parse_identifier(&parser)
		} else if parser.curr_token.kind == EOF {
			break
		} else if parser.curr_token.kind == CLOSEBRACE {
			if parser.is_function_opened {
				// if parser.statement_open >= 1 {
				// 	parser.statement_open -= 1
				// 	node := NodeAST{}
				// 	node.node_kind = END_WHILE_STATEMENT
				// 	node.node_kind_str = "EndWhileStatement"

				// 	append_node(&parser, node)
				// 	continue
				// }
				if parser.statement_open >= 1 {
					parser.statement_open -= 1
					node := NodeAST{}
					node.node_kind = END_STATEMENT
					node.node_kind_str = "EndStatement"
					if parser.last_statement_opened == "If" {
						node.end_statement_kind = IF_STATEMENT_END
					} else if parser.last_statement_opened == "Else" {
						node.end_statement_kind = ELSE_STATEMENT_END
					} else if parser.last_statement_opened == "Elif" {
						node.end_statement_kind = ELIF_STATEMENT_END
					} else if parser.last_statement_opened == "While" {
						parser.is_while_statement_opened--
						node.end_statement_kind = WHILE_STATEMENT_END
					} else if parser.last_statement_opened == "For" {
						parser.is_for_statement_opened--
						node.end_statement_kind = FOR_STATEMENT_END
					}

					append_node(&parser, node)
					continue
				} else {
					parser.is_function_opened = false
					reset_local_variables(&parser)
				}
			} else {
				barn_error_show_with_line(
					SYNTAX_ERROR, fmt.Sprintf("Unexpected use of `%s` token", kind_to_str(parser.curr_token.kind)),
					parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
					true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
				os.Exit(1)
			}
		} else {
			barn_error_show_with_line(
				SYNTAX_ERROR, fmt.Sprintf("Unexpected use of `%s` token", kind_to_str(parser.curr_token.kind)),
				parser.curr_token.filename, parser.curr_token.row, parser.curr_token.col-1,
				true, parser.lex.data_lines[parser.curr_token.filename_count][parser.curr_token.row-1])
			os.Exit(1)
		}
	}

	// for i := 0; i < len(parser.nodes); i++ {
	// 	fmt.Println(parser.nodes[i])
	// }

	return &parser
}
