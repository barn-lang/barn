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

#include <barn_struct.h>

#include <barn_variables.h>
#include <barn_core.h>
#include <barn_types.h>
#include <barn_parser.h>
#include <barn_expressions.h>
#include <barn_tokens.h>
#include <barn_functions.h>
#include <barn_type_checker.h>

size_t
barn_struct_calculate_size(barn_type_t* type)
{
    if (!barn_is_type_struct(type->type))
        barn_error_show(BARN_COMPILER_ERROR, "barn_struct_calculate_size: expected type structure");

    if (type->structure.struct_fields->length == 0)
        return 0;

    size_t structure_size = 0;
    BARN_ARRAY_FOR(type->structure.struct_fields)
    {
        barn_struct_field_t* field = barn_get_element_from_array(type->structure.struct_fields, i);
        structure_size += field->field_type->size;
    }

    return structure_size;
}

barn_type_t* 
barn_struct_type_create(char* name)
{
    barn_type_t* struct_type = (barn_type_t*)calloc(1, sizeof(barn_type_t));

    struct_type->structure.struct_fields    = barn_create_array(sizeof(barn_struct_field_t));
    struct_type->structure.sturct_type_name = (const char*)name;
    
    struct_type->type      = BARN_TYPE_STRUCT;
    struct_type->is_struct = barn_is_type_struct(struct_type->type);
    struct_type->size      = barn_struct_calculate_size(struct_type);

    return struct_type;
}

barn_struct_field_t*
barn_struct_create_new_field(barn_type_t* struct_type, char* name, barn_type_t* type)
{
    if (!barn_is_type_struct(struct_type->type))
        barn_error_show(BARN_COMPILER_ERROR, "barn_struct_create_new_field: expected type structure");

    barn_struct_field_t* field = (barn_struct_field_t*)calloc(1, sizeof(barn_struct_field_t));
    field->field_name = name;
    field->field_type = type;

    // Append a new field to our structure and recalculate 
    // structure size
    barn_append_element_to_array(struct_type->structure.struct_fields, field);
    struct_type->size = barn_struct_calculate_size(struct_type);

    return field;
}

const char*
barn_parser_collect_structure_name(barn_parser_t* parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        return NULL;

    const char* structure_name = barn_duplicate_string(parser->curr_token->value);
    if (!barn_parser_is_id_correct_namespace((char*)structure_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "structure can't be named \'%s\'", 
                        parser->curr_token->value);

    if (barn_parser_function_get_by_name(parser, (char*)structure_name) != NULL)
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "function with this name already exists", 0);

    if (barn_parser_is_variable_defined_g(parser, structure_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "variable with this name already exists", 0);

    if (barn_parser_is_structure_defined(parser, structure_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "structure with this name already exists", 0);

    return structure_name;
}

char*
barn_parser_collect_structure_field_name(barn_parser_t* parser)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        return NULL;

    const char* structure_field_name = barn_duplicate_string(parser->curr_token->value);
    if (!barn_parser_is_id_correct_namespace((char*)structure_field_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "structure field can't be named \'%s\'", 
                        parser->curr_token->value);

    return structure_field_name;
}

bool
barn_parser_is_structure_field_named(barn_parser_t* parser, barn_type_t* structure, char* field_name)
{
    BARN_ARRAY_FOR(structure->structure.struct_fields)
    {
        barn_struct_field_t* field = barn_get_element_from_array(
                                        structure->structure.struct_fields, i);

        if (BARN_STR_CMP(field->field_name, field_name))
            return true;
    }

    return false;
}

barn_struct_field_t*
barn_parser_get_structure_field_named(barn_parser_t* parser, barn_type_t* structure, char* field_name)
{
    BARN_ARRAY_FOR(structure->structure.struct_fields)
    {
        barn_struct_field_t* field = barn_get_element_from_array(
                                        structure->structure.struct_fields, i);

        if (BARN_STR_CMP(field->field_name, field_name))
            return field;
    }

    return NULL;
}

bool
barn_parser_struct_field_parse(barn_parser_t* parser, barn_type_t* structure)
{
    if (parser->curr_token->kind != BARN_TOKEN_IDENTIFIER)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected structure field name instead got \"%s\"", 
            barn_token_kind_to_string(parser->curr_token->kind));

    char* field_name = barn_parser_collect_structure_field_name(parser);

    if (barn_parser_is_structure_field_named(parser, structure, field_name))
        BARN_PARSER_ERR(parser, BARN_NAMESPACE_ERROR, "this name is already used by another field", 0);

    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind != BARN_TOKEN_COLON)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \":\" instead got \"%s\"", 
            barn_token_kind_to_string(parser->curr_token->kind));

    if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected identifier after colon to specify type of variable", 0);
    barn_parser_skip(parser, 1);

    if (BARN_STR_CMP(parser->curr_token->value, field_name))
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "using just implemented structure is not possible", 0);

    barn_type_t* field_type = barn_parser_current_token_type_representation(parser);
    if (field_type == NULL)
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "undefined field type \"%s\"", 
                        parser->curr_token->value);
    
    barn_struct_field_t* field = barn_struct_create_new_field(structure, field_name, field_type);

    if (barn_parser_is_next_token(parser, BARN_TOKEN_COMMA))
        barn_parser_skip(parser, 1);
    else
    {
        barn_parser_skip(parser, 1);
        return true; /* Stop parsing structure fields */
    }

    return false; /* Keep on going */
}

void
barn_parser_struct(barn_parser_t* parser) 
{
    if (barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"struct\" outside function body", 0);

    if (!barn_parser_is_next_token(parser, BARN_TOKEN_IDENTIFIER))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected structure type name", 0);

    barn_parser_skip(parser, 1);
    barn_type_t* struct_type = barn_struct_type_create(barn_parser_collect_structure_name(parser));

    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind != BARN_TOKEN_OPENBRACE)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected \"{\" after \"struct\" instead of \"%s\"", 
            barn_token_kind_to_string(parser->curr_token->kind));
    barn_parser_skip(parser, 1);

    if (parser->curr_token->kind == BARN_TOKEN_EOF)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected structure field not EOF", 0);

    while (parser->curr_token->kind != BARN_TOKEN_CLOSEBRACE)
    {
        // SKIP NEWLINES!!
        while (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            barn_parser_skip(parser, 1);

        if (barn_parser_struct_field_parse(parser, struct_type))
            break;

        // SKIP NEWLINES!!
        while (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            barn_parser_skip(parser, 1);

        barn_parser_skip(parser, 1);
    }

    while (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
        barn_parser_skip(parser, 1);

    if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
        barn_append_element_to_array(parser->structure_nodes, struct_type);
    else
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected '}' at the end of structure fields", 0);

    barn_node_t* structure = barn_create_empty_node(BARN_NODE_STRUCT);
    structure->structure.type_struct = struct_type;
    barn_parser_append_node(parser, structure);
}

bool
barn_parser_is_structure_defined(barn_parser_t* parser, char* struct_name)
{
    BARN_ARRAY_FOR(parser->structure_nodes)
    {
        barn_type_t* structure = barn_get_element_from_array(parser->structure_nodes, i);
        if (BARN_STR_CMP(structure->structure.sturct_type_name, struct_name))
            return true;
    }

    return false;
}

barn_type_t*
barn_parser_struct_find_by_name(barn_parser_t* parser, char* struct_name)
{
    BARN_ARRAY_FOR(parser->structure_nodes)
    {
        barn_type_t* structure = barn_get_element_from_array(parser->structure_nodes, i);
        if (BARN_STR_CMP(structure->structure.sturct_type_name, struct_name))
            return structure;
    }

    return NULL;
}
