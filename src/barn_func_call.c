/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2024
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

#include <barn_parser.h>
#include <barn_expressions.h>
#include <barn_functions.h>

barn_array_t*
barn_parse_function_call_arguments(barn_parser_t* parser)
{
    barn_array_t* func_call_args = barn_create_array(sizeof(barn_node_t*));

    barn_parser_skip(parser, 1);
    while (true)
    {
        if (parser->curr_token->kind == BARN_TOKEN_EOF)
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected value or \",\" to create argument list not EOF", 0);
        
        if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT)
            return func_call_args;

        barn_node_t* argument_value = barn_parse_expression(parser, BARN_TOKEN_COMMA, BARN_TOKEN_CLOSEPARENT, true);
        barn_append_element_to_array(func_call_args, argument_value);

        if (parser->curr_token->kind == BARN_TOKEN_COMMA)
        {
            barn_parser_skip(parser, 1);
            continue;
        }
        else if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT)
            return func_call_args;
        else
        {
            if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            {
                barn_parser_skip(parser, -1);
                if (parser->curr_token->kind == BARN_TOKEN_COMMA)
                {
                    barn_parser_skip(parser, 1);
                    continue;
                }
                else if (parser->curr_token->kind == BARN_TOKEN_CLOSEPARENT)
                    return func_call_args;
                else
                    BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \",\" or \")\" after argument value", 0);
            }
            else
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \",\" or \")\" after argument value", 0);
        }
    }

    // for (int i = 0; i < func_call_args->length; i++)
    // {
    //     barn_node_t* curr_args_node = barn_get_element_from_array(func_call_args, i);

    //     printf("func_call_args[%d]={ expression_nodes: %p, expression_nodes->length: %d }",
    //         curr_args_node->expression.expression_nodes,
    //         curr_args_node->expression.expression_nodes->length);
    // }

    return func_call_args;
}

void
barn_parser_func_call_arugments_length(barn_parser_t* parser, barn_node_t* mentioned_function, 
                                       barn_array_t* function_call_args)
{
    if (function_call_args->length != mentioned_function->function_declaration.function_args->length)
    {   
        if ((function_call_args->length < mentioned_function->function_declaration.function_args->length))
        {
            if (!((function_call_args->length + 1 == mentioned_function->function_declaration.function_args->length) &&
                   barn_parser_function_is_argument_list_with_format(mentioned_function->function_declaration.function_args)))
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "too few arguments to call function \"%s\", expected %d, got %d",
                    mentioned_function->function_declaration.function_name,
                    mentioned_function->function_declaration.function_args->length,
                    function_call_args->length);
        }
        else
        {
            if (!barn_parser_function_is_argument_list_with_format(mentioned_function->function_declaration.function_args))
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "too many arguments to call function \"%s\", expected %d, got %d",
                    mentioned_function->function_declaration.function_name,
                    mentioned_function->function_declaration.function_args->length,
                    function_call_args->length);
        }
    }
}

barn_node_t*
__barn_parser_func_call(barn_parser_t* parser, bool is_expr)
{
    const char* function_call_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    barn_token_t* function_token   = parser->curr_token;

    if (!BARN_STR_CMP(function_call_name, BARN_FUNCTION_INJECTING_CODE))
        if (!is_expr)
            if (!barn_parser_is_function_opened(parser))
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to call a function", 0);

    barn_node_t* mentioned_function = barn_parser_function_get_by_name(parser, function_call_name);

    if (mentioned_function == NULL)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function named \"%s\" doesn't exists", function_call_name);
        
    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind != BARN_TOKEN_OPENPARENT)
        BARN_UNIMPLEMENTED("this shouldn't happend");

    barn_array_t* function_call_args = NULL;

    if (barn_parser_is_next_token(parser, BARN_TOKEN_CLOSEPARENT))
    {
        function_call_args = barn_create_array(sizeof(void*));
        barn_parser_skip(parser, 1);
    }
    else
        function_call_args = barn_parse_function_call_arguments(parser);

    // for (int i = 0; i < function_call_args->length; i++)
    // {
    //     barn_node_t* curr_args_node = barn_get_element_from_array(function_call_args, i);

    //     printf("func_call_args[%d]={ expression_nodes: %p, expression_nodes->length: %d }", i,
    //         curr_args_node->expression.expression_nodes,
    //         curr_args_node->expression.expression_nodes->length);
    // }

    barn_parser_func_call_arugments_length(parser, mentioned_function, function_call_args);

    barn_node_t*  function_call_node = barn_create_empty_node(BARN_NODE_FUNCTION_CALL);
    
    function_call_node->function_call.function_token = function_token;
    function_call_node->function_call.function_name  = function_call_name;
    function_call_node->function_call.function_args  = function_call_args;
    function_call_node->function_call.function       = mentioned_function;
    return function_call_node;
}

void 
barn_parser_func_call(barn_parser_t* parser, bool is_expr)
{
    barn_node_t* function_call_node = __barn_parser_func_call(parser, is_expr);
    barn_parser_append_node(parser, function_call_node);
}