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

#include <barn_variable_modification.h>
#include <barn_core.h>

#include <barn_variables.h>
#include <barn_parser.h>
#include <barn_types.h>
#include <barn_nodes.h>

void
barn_parser_variable_asn(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_token_t* start_token = parser->curr_token;
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);
    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);
    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASN);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = start_token;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_plusasn(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_token_t* start_token = parser->curr_token;
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);
    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNPLUS);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = start_token;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_modasn(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_token_t* start_token = parser->curr_token;
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);
    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNMOD);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = start_token;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_minusasn(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_token_t* start_token = parser->curr_token;
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);
    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNMINUS);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = start_token;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_mulasn(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_token_t* start_token = parser->curr_token;
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);
    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNMUL);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = start_token;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_divasn(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_token_t* start_token = parser->curr_token;
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be modified", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* new_var_value = barn_parse_expression(parser, BARN_TOKEN_NEWLINE, BARN_TOKEN_SEMICOL, true);
    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_ASNDIV);
    node->variable_modification.variable_value = new_var_value;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = start_token;

    barn_parser_append_node(parser, node);
}

void 
barn_parser_variable_incrementation(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be incremeneted", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_INCREMENTATION);
    node->variable_modification.variable_value = NULL;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = parser->curr_token;

    barn_parser_append_node(parser, node);
} 

void 
barn_parser_variable_decrementation(barn_parser_t* parser, barn_parser_access_element_t* element)
{
    barn_parser_skip(parser, 1);
    
    if (element->variable.variable->is_const == true)
        BARN_PARSER_ERR(parser, BARN_PARSER_ERROR, "'%s' is a constant variable that can't be decremented", 
                        parser->curr_token->value);

    barn_parser_skip(parser, 1);
    element->variable.variable->is_used = true;

    barn_node_t* node = barn_create_empty_node(BARN_NODE_VARIABLE_DECREMENTATION);
    node->variable_modification.variable_value = NULL;
    node->variable_modification.access_element = element;
    node->variable_modification.variable_token = parser->curr_token;

    barn_parser_append_node(parser, node);
}