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

#include <barn_variable_modification.h>
#include <barn_core.h>

#include <barn_variables.h>
#include <barn_parser.h>
#include <barn_types.h>
#include <barn_nodes.h>

void
barn_parser_variable_asn(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 2);

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASN);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.variable       = variable;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_plusasn(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 2);

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNPLUS);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.variable       = variable;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_minusasn(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 2);

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNMINUS);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.variable       = variable;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_mulasn(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 2);

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNMUL);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.variable       = variable;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_divasn(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 2);

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNDIV);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.variable       = variable;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_incrementation(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be incremented", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_INCREMENTATION);
    node->variable_modification.variable_value = NULL;
    node->variable_modification.variable       = variable;
    node->variable_modification.variable_token = parser->curr_token;

    barn_parser_append_node(parser, node);
} 

void 
barn_parser_variable_decrementation(barn_parser_t* parser)
{
    const char* variable_name = BARN_COLLECT_CURR_TOK_VAL(parser);
    
    if (!barn_parser_is_variable_defined_lg(parser, variable_name))
        BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "'%s' is undefined", 
                        parser->curr_token->value);

    barn_variable_t* variable = barn_parser_get_variable_by_name(parser, variable_name);
    if (variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be decremented", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_DECREMENTATION);
    node->variable_modification.variable_value = NULL;
    node->variable_modification.variable       = variable;
    node->variable_modification.variable_token = parser->curr_token;

    barn_parser_append_node(parser, node);
}