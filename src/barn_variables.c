/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2023
 *   
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
*/

#include <barn_variables.h>

#include <barn_core.h>
#include <barn_types.h>

#include <barn_parser.h>
#include <barn_expressions.h>
#include <barn_tokens.h>
#include <barn_functions.h>
#include <barn_type_checker.h>

barn_variable_t* 
barn_create_variable(const char* var_name, barn_type_t* var_type, 
                     bool is_const, bool is_used, bool is_static)
{
    barn_variable_t* var = (barn_variable_t*)calloc(1, sizeof(barn_variable_t));

    var->is_const  =  is_const ;
    var->is_used   =  is_used  ;
    var->is_static =  is_static;
    var->var_name  =  var_name ;
    var->var_type  =  var_type ;

    return var;
}

// barn_parser_is_variable_defined_lg function is checking does
// a variable named variable_name already exists
//
// lg stands for "local and global"
bool
barn_parser_is_variable_defined_lg(barn_parser_t* parser, const char* variable_name)
{
    if (barn_parser_is_variable_defined_l(parser, variable_name))
        return true;
    
    if (barn_parser_is_variable_defined_g(parser, variable_name))
        return true;

    return false;
}

// barn_parser_is_variable_defined_lg function is checking does
// a variable named variable_name already exists
//
// l stands for "local"
bool
barn_parser_is_variable_defined_l(barn_parser_t* parser, const char* variable_name)
{  
    BARN_ARRAY_FOR(parser->local_variables)
    {
        if (BARN_STR_CMP(
                ((barn_variable_t*)barn_get_element_from_array(parser->local_variables, i))
                ->var_name, variable_name))
            return true;
    }

    return false;
}

// barn_parser_is_variable_defined_lg function is checking does
// a variable named variable_name already exists
//
// g stands for "global"
bool
barn_parser_is_variable_defined_g(barn_parser_t* parser, const char* variable_name)
{
    BARN_ARRAY_FOR(parser->global_variables)
    {
        if (BARN_STR_CMP(
                ((barn_variable_t*)barn_get_element_from_array(parser->global_variables, i))
                ->var_name, variable_name))
            return true;
    }

    return false; 
}

const char*
barn_parser_collect_variable_name(barn_parser_t* parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        return NULL;

    const char* function_name = barn_duplicate_string(parser->curr_token->value);
    if (!barn_parser_is_id_correct_namespace((char*)function_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "variable couldn't be named \'%s\'", 
                        parser->curr_token->value);

    if (barn_parser_function_get_by_name(parser, (char*)function_name) != NULL)
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "this name is already took by a function", 0);

    return function_name;
}

void
barn_parser_variable_declaration(barn_parser_t* parser, bool is_constant, bool is_static)
{
    // let/const example_var: type = <expression>
    //            ^
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected function name after 'func' keyword", 0);
    barn_parser_skip(parser, 1);

    const char* variable_name = barn_parser_collect_variable_name(parser);

    if (barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "variable named '%s' already exists", variable_name);

    // let/const example_var: type = <expression>
    //                      ^
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_COLON))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "expected colon after variable name to specify type of variable", 0);
    barn_parser_skip(parser, 1);

    // let/const example_var: type = <expression>
    //                         ^
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "expected identifier after colon to specify type of variable", 0);
    barn_parser_skip(parser, 1);

    barn_type_t* variable_type = barn_parser_current_token_type_representation(parser);
    if (variable_type == NULL)
    {
        if (strcmp(parser->curr_token->value, "auto") == 0)
            variable_type = barn_create_type(BARN_TYPE_AUTO);
        else
            BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "undefined variable type '%s'", 
                            parser->curr_token->value);
    }


    // let/const example_var: type = <expression>
    //                                ^
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_ASN))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "expected '=' token to specify variable value", 0);
    barn_parser_skip(parser, 2);
 
    barn_node_t* variable_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);

    barn_variable_t* variable = barn_create_variable(variable_name, variable_type, is_constant, false, is_static);

    if (variable->var_name[0] == '_')
        variable->is_used = true;

    barn_node_t* variable_decl = barn_create_empty_node(BARN_NODE_VARIABLE_DECLARATION);
    variable_decl->variable_declaration.variable_value = variable_value;
    variable_decl->variable_declaration.variable       = variable;
    barn_parser_append_node(parser, variable_decl);

    if (barn_parser_is_function_opened(parser))
        barn_append_element_to_array(parser->local_variables, variable);
    else
        barn_append_element_to_array(parser->global_variables, variable);
}

barn_variable_t*
barn_parser_get_variable_by_name(barn_parser_t* parser, const char* variable_name)
{
    BARN_ARRAY_FOR(parser->global_variables)
    {
        if (BARN_STR_CMP(
                ((barn_variable_t*)barn_get_element_from_array(parser->global_variables, i))
                ->var_name, variable_name))
            return ((barn_variable_t*)barn_get_element_from_array(parser->global_variables, i));
    }

    BARN_ARRAY_FOR(parser->local_variables)
    {
        if (BARN_STR_CMP(
                ((barn_variable_t*)barn_get_element_from_array(parser->local_variables, i))
                ->var_name, variable_name))
            return ((barn_variable_t*)barn_get_element_from_array(parser->local_variables, i));
    }

    return NULL;
}

void
barn_parser_static_variable_declaration(barn_parser_t* parser)
{
    barn_parser_skip(parser, 1);

    if (parser->curr_token->kind != BARN_TOKEN_IDENTIFIER)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"const\" or \"let\" keyword after \"static\"", 0);

    if ((BARN_TOKEN_CMP("let") || BARN_TOKEN_CMP("const")) == false)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"const\" or \"let\" keyword after \"static\"", 0);

    barn_parser_variable_declaration(parser, BARN_TOKEN_CMP("const"), true);
}