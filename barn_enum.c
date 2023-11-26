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

barn_enum_field_t* 
barn_create_enum_field(const char* enum_name, barn_node_t* enum_expression) 
{
    barn_enum_field_t* enum_field = calloc(1, sizeof(barn_enum_field_t));
    enum_field->enum_expression   = enum_expression;
    enum_field->enum_name         = enum_name;

    return enum_field;
}

barn_array_t* 
barn_parser_enum_fields(barn_parser_t* parser)
{
    barn_parser_skip(parser, 1);
    barn_array_t* enum_fields;

    while (parser->curr_token->kind != BARN_TOKEN_CLOSEBRACE || parser->curr_token->kind != BARN_TOKEN_EOF)
    {
        if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected enum field name", 0);

        barn_parser_skip(parser, 1);    
        barn_enum_field_t* field = barn_create_enum_field(parser->curr_token->value, NULL);

        if (parser->curr_token->kind == BARN_TOKEN_COMMA)
        {
            barn_parser_skip(parser, 1);
            barn_append_element_to_array(enum_fields, field);
            continue;
        } 
        else if (parser->curr_token->kind == BARN_TOKEN_EQ)
            BARN_UNIMPLEMENTED("enum field values");
        else if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
        {
            barn_append_element_to_array(enum_fields, field);
            break;
        }
        else 
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"=\", \",\" or \"{\" not \"%s\"", 
                barn_token_kind_to_string(parser->curr_token->kind));
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
    {
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after \"enum\" instead of \"%s\"", 
            barn_token_kind_to_string(parser->curr_token->kind));
    }

    barn_parser_enum_fields(parser);
}