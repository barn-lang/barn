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

#include <barn_enum.h>

#include <barn_core.h>
#include <barn_parser.h>
#include <barn_functions.h>
#include <barn_struct.h>

barn_enum_field_t* 
barn_create_enum_field(const char* enum_name, barn_node_t* enum_expression) 
{
    barn_enum_field_t* enum_field = calloc(1, sizeof(barn_enum_field_t));
    enum_field->enum_expression   = enum_expression;
    enum_field->enum_name         = enum_name;

    return enum_field;
}

char*
barn_parser_collect_enum_field_name(barn_parser_t* parser)
{
    if (parser->curr_token->kind != BARN_TOKEN_IDENTIFIER)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected enum field name as an identifier", 0);
    
    char* field_name = barn_duplicate_string(parser->curr_token->value);
    if (!barn_parser_is_id_correct_namespace((char*)field_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "enum field can't be named \'%s\'", 
                        parser->curr_token->value);

    if (barn_parser_function_get_by_name(parser, field_name) != NULL)
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "this name is already took by a function", 0);

    if (barn_parser_is_structure_defined(parser, field_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "this name is already took by a structure", 0);

    return field_name;
}

void
barn_parser_enum_append_field(barn_parser_t* parser, barn_array_t* enum_fields,
                              char* field_name, barn_node_t* field_val)
{
    // Create a global variable named by field_name
    barn_append_element_to_array(enum_fields, barn_create_enum_field(field_name, field_val));
    barn_parser_skip(parser, 1);

    barn_variable_t* variable 
        = barn_create_variable(field_name, barn_get_type_i32_global(), true, true, false);
    barn_append_element_to_array(parser->global_variables, variable);
}

barn_array_t* 
barn_parser_enum_fields(barn_parser_t* parser)
{
    barn_array_t* enum_fields = barn_create_array(sizeof(barn_enum_field_t));
    
    if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
        barn_parser_skip(parser, 1);

    while (parser->curr_token->kind != BARN_TOKEN_CLOSEBRACE || parser->curr_token->kind != BARN_TOKEN_EOF)
    {
        while (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            barn_parser_skip(parser, 1);

        char* field_name = barn_parser_collect_enum_field_name(parser);
        barn_parser_skip(parser, 1);

        while (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            barn_parser_skip(parser, 1);

        if (parser->curr_token->kind == BARN_TOKEN_COMMA)
        {
            barn_parser_enum_append_field(parser, enum_fields, field_name, NULL);
            continue;
        }
        else if (parser->curr_token->kind == BARN_TOKEN_ASN)
        {
            // Create a global variable named by field_name and assign 
            // a value from parsed expression
            barn_parser_skip(parser, 1);
            barn_node_t* field_expression = barn_parse_expression(parser, BARN_TOKEN_COMMA, BARN_TOKEN_CLOSEBRACE, false);
            
            if (field_expression->expression.is_compiler_time == false && !barn_parser_is_function_opened(parser))
                BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "value is not a compile-time expression", 0);

            barn_parser_enum_append_field(parser, enum_fields, field_name, field_expression);

            if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
                break;
            
            barn_parser_skip(parser, 1);
            continue;
        }
        else if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
        {
            barn_parser_enum_append_field(parser, enum_fields, field_name, NULL);
            break;
        }
        else
            BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "expected \",\", \"=\" or \"}\" after enum field name", 0);
    }
    
    return enum_fields;
}

void 
barn_parser_enum(barn_parser_t* parser)
{
    if (barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"enum\" outside function body", 0);

    barn_parser_skip(parser, 1);

    if (parser->curr_token->kind != BARN_TOKEN_OPENBRACE)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after \"enum\" instead of \"%s\"", 
            barn_token_kind_to_string(parser->curr_token->kind));
        
    barn_parser_skip(parser, 1);

    // If it's something like this: `enum {}` just return..
    // we don't need to do anything with it
    if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
        return;

    barn_node_t* enum_node           = barn_create_empty_node(BARN_NODE_ENUM);
    enum_node->enumerate.enum_fields = barn_parser_enum_fields(parser);
    barn_parser_append_node(parser, enum_node);
}