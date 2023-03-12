package main

import "fmt"

type BarnTypes int

const (
	BARN_U8  BarnTypes = iota // Unsigned char 
	BARN_U16  // Unsigned short
	BARN_U32  // Unsigned int
	BARN_U64  // Unsigned long 

	BARN_I8   // Char
	BARN_I16  // Short
	BARN_I32  // Int
	BARN_I64  // Long
	
	BARN_F32  // Float
	BARN_F64  // Double

	BARN_STR  // String
	BARN_BOOL // Bool
	
	BARN_AUTO // Auto
	BARN_ANY  // Any
	BARN_TYPE_NONE = -1
)

const (
	/* Function stuff */
	FUNCTION_DECLARATION = iota
	FUNCTION_RETURN
	FUNCTION_CALL

	/* Import C */
	IMPORT_C

	/* Variable declaration, assignment */
	VARIABLE_DECLARATION
	VARIABLE_ASSIGNMENT

	/* Variable stuff */
	VARIABLE_PLUS_ASSIGNMENT
	VARIABLE_MINUS_ASSIGNMENT
	VARIABLE_MUL_ASSIGNMENT
	VARIABLE_DIV_ASSIGNMENT
	VARIABLE_INCREMENTATION
	VARIABLE_DECREMENTATION

	/* If, elif, else statements */
	IF_STATEMENT
	ELSE_STATEMENT
	ELSE_IF_STATEMENT

	/* While & for statements */
	WHILE_STATEMENT
	FOR_STATEMENT
	CONTINUE_STATEMENT
	BREAK_STATEMENT

	/* End statement */
	END_STATEMENT
)

const (
	IF_STATEMENT_END int = iota
	ELSE_STATEMENT_END
	ELIF_STATEMENT_END
	WHILE_STATEMENT_END
	FOR_STATEMENT_END
)

type ArgsFunctionDeclaration struct {
	name     string
	type_arg BarnTypes
}

type ArgsFunctionCall struct {
	name     string
	type_arg BarnTypes
	value    string
	is_var   bool
}

type NodeAST struct {
	/* NodeAST properties */
	node_kind     int
	node_kind_str string

	/* Function declaration properties */
	function_name   string
	function_args   []ArgsFunctionDeclaration
	function_return BarnTypes
	function_body   []*NodeAST

	/* Last token of expression */
	last_node_token *Token

	/* Function call properties */
	call_name string
	call_args []ArgsFunctionCall
	call_func *NodeAST

	/* Import c properties */
	import_c_header string

	/* Variable declaration properties */
	variable_name          string
	variable_type          BarnTypes
	variable_value         string
	variable_is_arg        bool
	variable_fn_call_value bool
	variable_constant      bool
	variable_used          bool

	/* Variable assignment properties */
	variable_assignment_name  string
	variable_assignment_value string

	/* Variable plus assignment properties */
	variable_plus_assignment_name  string
	variable_plus_assignment_value string

	/* Variable minus assignment properties */
	variable_minus_assignment_name  string
	variable_minus_assignment_value string

	/* Variable mul assignment properties */
	variable_mul_assignment_name  string
	variable_mul_assignment_value string

	/* Variable div assignment properties */
	variable_div_assignment_name  string
	variable_div_assignment_value string

	/* Function return properties */
	function_return_value string

	/* If statement */
	if_condition string

	/* Else if statement */
	else_if_condition string

	/* While statement */
	while_condition string

	/* End statement */
	end_statement_kind int

	/* For statement */
	for_var_declaration *NodeAST
	for_condition       string
	for_var_operation   *NodeAST
}

func (n *NodeAST) show() {
	fmt.Printf("%s", n.node_kind_str)
}

func (barn_type BarnTypes) as_string() string {
	switch barn_type {
	case BARN_BOOL:
		return "bool"
	case BARN_I32:
		return "int"
	case BARN_F32:
		return "float"
	case BARN_I8:
		return "char"
	case BARN_STR:
		return "string"
	case BARN_AUTO:
		return "auto"
	case BARN_TYPE_NONE:
		return "none"
	}

	return "none"
}
