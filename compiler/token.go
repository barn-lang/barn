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

const (
	/* Special tokens */
	NONE = iota
	EOF

	/* Default tokens */
	INT
	CHAR
	FLOAT
	STRING
	IDENTIFIER

	/* Operators */
	PLUS           // "+"
	MINUS          // "-"
	MUL            // "*"
	DIV            // "/"
	MOD            // "%"
	BANG           // "!"
	PLUSASN        // "+="
	MINUSASN       // "-="
	MULASN         // "*="
	DIVASN         // "/="
	INCREMENTATION // "++"
	DECREMENTATION // "--"

	/* Logical Operators */
	GT  // ">"
	GTE // ">="
	LT  // "<"
	LTE // "<="
	ASN // "="
	EQ  // "=="
	NEQ // "!="

	/* Btiwise operators */
	OROR   // "||"
	ANDAND // "&&"

	/* Structural tokens */
	OPENPARENT     // "("
	CLOSEPARENT    // ")"
	OPENBRACKET    // "["
	CLOSEBRACKET   // "]"
	OPENBRACE      // "{"
	CLOSEBRACE     // "}"
	CONDITIONBLOCK // "|"

	/* Seperators */
	COMMA   // "," 
	COLON   // ":"
	DOT     // "."
	SEMICOL // ";"

	/* Arrows */
	ARROW // "->"
	
	/* Other */
	TRIPLEDOT  // "..."
)

type Token struct {
	value          string
	filename       string
	line           string
	col            int
	row            int
	kind           int
	filename_count int
}

func kind_to_str(kind int) string {
	switch kind {
	case NONE:
		return "NONE"
	case EOF:
		return "EOF"
	case INT:
		return "INT"
	case CHAR:
		return "CHAR"
	case FLOAT:
		return "FLOAT"
	case STRING:
		return "STRING"
	case IDENTIFIER:
		return "IDENTIFIER"
	case PLUS:
		return "PLUS"
	case MINUS:
		return "MINUS"
	case MUL:
		return "MUL"
	case DIV:
		return "DIV"
	case PLUSASN:
		return "PLUSASN"
	case MINUSASN:
		return "MINUSASN"
	case MULASN:
		return "MULASN"
	case DIVASN:
		return "DIVASN"
	case BANG:
		return "BANG"
	case GT:
		return "GT"
	case GTE:
		return "GTE"
	case LT:
		return "LT"
	case LTE:
		return "LTE"
	case ASN:
		return "ASN"
	case EQ:
		return "EQ"
	case NEQ:
		return "NEQ"
	case OROR:
		return "OROR"
	case ANDAND:
		return "ANDAND"
	case OPENPARENT:
		return "OPENPARENT"
	case CLOSEPARENT:
		return "CLOSEPARENT"
	case OPENBRACKET:
		return "OPENBRACKET"
	case CLOSEBRACKET:
		return "CLOSEBRACKET"
	case OPENBRACE:
		return "OPENBRACE"
	case CLOSEBRACE:
		return "CLOSEBRACE"
	case COMMA:
		return "COMMA"
	case COLON:
		return "COLON"
	case DOT:
		return "DOT"
	case SEMICOL:
		return "SEMICOL"
	case CONDITIONBLOCK:
		return "CONDITIONBLOCK"
	case MOD:
		return "MOD"
	case ARROW:
		return "ARROW"
    case TRIPLEDOT:
        return "TRIPLEDOT"
	}

	return "NONE"
}

func create_token(line string, filename_count int, value string, filename string, col int, row int, kind int) Token {
	var tk Token
	tk.value		  = value
	tk.filename 	  = filename
	tk.col 			  = col
	tk.row 			  = row
	tk.kind 		  = kind
	tk.filename_count = filename_count
	tk.line 		  = line
	return tk
}

func print_token(tk Token) {
	fmt.Printf("{ value: `%s`, row: %d, col: %d, kind: `%s`, filename: `%s` }\n", tk.value, tk.row, tk.col, kind_to_str(tk.kind), tk.filename)
}
