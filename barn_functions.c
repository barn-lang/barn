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

#include <barn_core.h>

#include <barn_functions.h>
#include <barn_parser.h>
#include <barn_string.h>
#include <barn_lexer.h>
#include <barn_nodes.h>
#include <barn_array.h>
#include <barn_types.h>
#include <barn_io.h>

#ifndef BARN_TOKEN_CMP
# define BARN_TOKEN_CMP(str) (strcmp(parser->curr_token->value, str) == 0)
#endif /* BARN_TOKEN_CMP */

#ifndef BARN_PARSER_ERR
# define BARN_PARSER_ERR(parser, error_type, msg, ...) ({            \
     barn_error_show_with_line(parser->lexer,                        \
         error_type, parser->curr_token->filename,                   \
         parser->curr_token->row - 1, parser->curr_token->col - 1,   \
         true, parser->curr_token->line, msg, __VA_ARGS__);          \
     exit(1);                                                        \
 })
#endif /* BARN_PARSER_ERR */

barn_node_t*
barn_parser_function_get_by_name(barn_parser_t* parser, char* function_name)
{
    for (int i = 0; i < parser->function_nodes->length; i++)
    {
        barn_node_t* node = barn_get_element_from_array(parser->function_nodes, i);
        
        if (node->node_kind != BARN_NODE_FUNCTION_DECLARATION)
            BARN_UNIMPLEMENTED("unknown node_kind that shouldn't be here");

        if (BARN_STR_CMP(node->function_declaration.function_name, function_name))
            return node;
    }

    return NULL;
}

/* 
 * This function named `barn_parser_collect_function_name`
 * is a very interesting one because it first of all checks
 * is the current token EOF kind next we duplicate this string
 * and checks is it a correct namespace and does a function
 * like this doesn't exists already 
 */
const char*
barn_parser_collect_function_name(barn_parser_t* parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        return NULL;

    const char* function_name = barn_duplicate_string(parser->curr_token->value);
    if (!barn_parser_is_id_correct_namespace((char*)function_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "function couldn't be named \'%s\'", 
                        parser->curr_token->value);

    if (barn_parser_function_get_by_name(parser, (char*)function_name) != NULL)
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "function with this name already exists", 0);

    // TODO: does function name equal to variable name

    return function_name;
}

bool
barn_parser_function_args_parse_is_close_parent(barn_parser_t* parser, 
                                                barn_parse_function_args_t* func_args_parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT && !func_args_parser->expect_name)
    {
        barn_parser_skip(parser, 1);
        return true;
    }

    return false;
}

void
barn_parser_function_args_parse_type(barn_parser_t* parser, barn_parse_function_args_t* func_args_parser, barn_array_t* args)
{
    if (parser->curr_token->kind == BARN_TOKEN_IDENTIFIER)
    {
        barn_type_t* argument_type = barn_parser_current_token_type_representation(parser);

        if (argument_type == NULL)
            BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "undefined and unknown type name '%s'", 
                            parser->curr_token->value);

        barn_append_element_to_array(args, barn_create_func_argument(argument_type, parser->curr_token->value));
        
        func_args_parser->expect_type  = false;
        func_args_parser->expect_name  = true;
        func_args_parser->expect_comma = false;

        barn_parser_skip(parser, 1);
        return;
    } 
    else if (parser->curr_token->kind == BARN_TOKEN_TRIPLEDOT)
    {  
        // TODO: implement tripledot arguments
        BARN_UNIMPLEMENTED("unimplemented tripledot arguments");
        return;
    }

    BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected type name or triple dot token", 0);
}

void
barn_parser_function_args_parse_name(barn_parser_t* parser, barn_parse_function_args_t* func_args_parser, barn_array_t* args)
{
    if (parser->curr_token->kind != BARN_TOKEN_IDENTIFIER)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected argument name after correct type name", 0);

    const char* argument_name = parser->curr_token->value;

    for (int i = 0; i < args->length; i++)
        if (BARN_STR_CMP((char*)barn_get_element_from_array(args, i), argument_name))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "given argument name is already in use", 0);

    barn_func_argument_t* last_argument = barn_get_element_from_array(args, args->length - 1);
    last_argument->argument_name = barn_duplicate_string(argument_name);

    func_args_parser->expect_comma = true;
    func_args_parser->expect_name  = false;
    func_args_parser->expect_type  = false;

    barn_parser_skip(parser, 1);
    return;
}

void
barn_parser_function_args_parse_comma(barn_parser_t* parser, barn_parse_function_args_t* func_args_parser, barn_array_t* args)
{
    if (parser->curr_token->kind != BARN_TOKEN_COMMA)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected comma sign ',' after argument name to seperate it", 0);

    func_args_parser->expect_comma = false;
    func_args_parser->expect_type  = true;
    func_args_parser->expect_name  = false;

    barn_parser_skip(parser, 1);
    return;
}

barn_func_argument_t* 
barn_create_func_argument(barn_type_t* argument_type, const char* argument_name)
{
    barn_func_argument_t* argument = (barn_func_argument_t*)calloc(1, sizeof(barn_func_argument_t));
    argument->argument_name = argument_name;
    argument->argument_type = argument_type;

    return argument;
}

void
barn_function_declaration_set_argument_as_variables(barn_parser_t* parser, barn_node_t* node)
{
    for (int i = 0; i < node->function_declaration.function_args->length; i++)
    {
        // TODO: implement setting arguments as local variables 
    }
}

barn_array_t*
barn_parser_function_args_parse(barn_parser_t* parser) 
{
    barn_array_t* args = barn_create_array(sizeof(barn_func_argument_t));

    if (barn_parser_is_next_token(parser, BARN_TOKEN_CLOSEPARENT))
    {
        barn_parser_skip(parser, 1);
        return args;
    }

    if (barn_parser_is_next_token(parser, BARN_TOKEN_EOF))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected next token no EOF", 0);

    barn_parser_skip(parser, 1);

    barn_parse_function_args_t func_args_parser = {
        .expect_type = true,
        .expect_name = false,
        .expect_comma = false,
        .index = parser->index,
    };

    for (; func_args_parser.index < parser->lexer->tokens->length; func_args_parser.index++)
    {
        if (barn_parser_function_args_parse_is_close_parent(parser, &func_args_parser))
            break;

        if      (func_args_parser.expect_type == true && func_args_parser.expect_name == false && func_args_parser.expect_comma == false)
            barn_parser_function_args_parse_type(parser, &func_args_parser, args);
        else if (func_args_parser.expect_type == false && func_args_parser.expect_name == true && func_args_parser.expect_comma == false)
            barn_parser_function_args_parse_name(parser, &func_args_parser, args);
        else if (func_args_parser.expect_type == false && func_args_parser.expect_name == false && func_args_parser.expect_comma == true)
            barn_parser_function_args_parse_comma(parser, &func_args_parser, args);
    }

    return args;
}

barn_node_t*
barn_function_declaration_create_node(const char* function_name, barn_array_t* function_args,
                                      barn_type_t* function_return_type)
{
    barn_node_t* function_decl = barn_create_empty_node(BARN_NODE_FUNCTION_DECLARATION);

    function_decl->function_declaration.function_name   = function_name;
    function_decl->function_declaration.function_args   = function_args;
    function_decl->function_declaration.function_return = function_return_type;
    function_decl->function_declaration.function_nodes  = barn_create_array(sizeof(barn_node_t*));

    return function_decl;
}

void
barn_function_declaration_set_values(barn_parser_t* parser, barn_node_t* func)
{
    parser->actual_function = func;
    barn_append_element_to_array(parser->function_nodes, func);
}

void
barn_parser_function_declaration(barn_parser_t* parser)
{
    if (barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function is already opened", 0);

    if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected function name after 'func' keyword", 0);
    barn_parser_skip(parser, 1);

    const char* function_name = barn_parser_collect_function_name(parser);

    if (!barn_parser_is_next_token(parser, BARN_TOKEN_OPENPARENT))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected '(' after function name", 0);
    barn_parser_skip(parser, 1);

    barn_array_t* function_args = barn_parser_function_args_parse(parser);

    if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT)
        barn_parser_skip(parser, 1);

    for (int i = 0; i < function_args->length; i++)
        printf("[%d]=argument_name:\"%s\", argument_type->size:\"%lu\"\n", 
            i, ((barn_func_argument_t*)barn_get_element_from_array(function_args, i))->argument_name,
            ((barn_func_argument_t*)barn_get_element_from_array(function_args, i))->argument_type->size);

    if (parser->curr_token->kind == BARN_TOKEN_EOF)
    {
        barn_parser_skip(parser, -1);
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected '{' or '->' after declaring a function not EOF", 0);
    }

    if (parser->curr_token->kind != BARN_TOKEN_OPENBRACE && parser->curr_token->kind != BARN_TOKEN_ARROW)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected '{' or '->' after declaring a function", 0);

    barn_type_t* function_return_type = barn_create_type(BARN_TYPE_NONE);
    if (parser->curr_token->kind == BARN_TOKEN_ARROW)
    {
        barn_parser_skip(parser, 1);
        if (parser->curr_token->kind != BARN_TOKEN_IDENTIFIER)
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected type name after '->' arrow", 0);

        function_return_type = barn_parser_current_token_type_representation(parser);
        if (function_return_type == NULL)
            BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "undefined and unknown type name '%s'", 
                            parser->curr_token->value);
        
        barn_parser_skip(parser, 1);
    }

    if (parser->curr_token->kind != BARN_TOKEN_OPENBRACE)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected '{' to open function body", 0);

    barn_node_t* node = barn_function_declaration_create_node(function_name, function_args, 
                                                              function_return_type);
    barn_parser_append_node(parser, node);
    barn_function_declaration_set_values(parser, node);
    barn_function_declaration_set_argument_as_variables(parser, node);
}