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

#include <barn_func_return.h>
#include <barn_functions.h>
#include <barn_func_call.h>

#include <barn_struct.h>
#include <barn_enum.h>

#include <barn_string.h>
#include <barn_array.h>
#include <barn_io.h>

#include <barn_variable_modification.h>
#include <barn_variables.h>

#include <barn_conditions.h>
#include <barn_loop.h>

#include <barn_lexer.h>
#include <barn_nodes.h>
#include <barn_warning.h>

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

#define BARN_KEYWORDS_LEN 17

static const char* barn_const_keywords[BARN_KEYWORDS_LEN] = {
    [0 ] = "fun",
    [1 ] = "extern",
    [2 ] = "@import_c", // to implement
    [3 ] = "@import",
    [4 ] = "let",
    [5 ] = "const",
    [6 ] = "return",
    [7 ] = "if",
    [8 ] = "else",
    [9 ] = "elif",
    [10] = "while",
    [11] = "continue",
    [12] = "break",
    [13] = "for",
    [14] = "struct", // to implement
    [15] = "static",
    [16] = "enum", // to implement
};

/* Function for skipping tokens */
void 
barn_parser_skip(barn_parser_t* parser, int n)
{
    parser->index += n;
    parser->curr_token = barn_get_element_from_array(parser->lexer->tokens, parser->index);

    // if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
    //     barn_parser_skip(parser, 1);
}

bool
barn_parser_is_next_token(barn_parser_t* parser, barn_token_kind_t kind)
{
    if (parser->curr_token->kind == BARN_TOKEN_EOF)
    {
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expeceted '%s' not EOF", 
                        barn_token_kind_to_string(kind));
    }

    barn_parser_skip(parser, 1);
    if (parser->curr_token->kind == kind)
    {
        barn_parser_skip(parser, -1);
        return true;
    }

    barn_parser_skip(parser, -1);
    return false;
}

bool
barn_parser_is_id_keyword(char* id_keyword)
{
    for (int i = 0; i < BARN_KEYWORDS_LEN; i++)
        if (BARN_STR_CMP(barn_const_keywords[i], id_keyword))
            return true;

    return false;
}

bool
barn_parser_is_id_correct_namespace(char* id_namespace)
{
    if (barn_parser_is_id_keyword(id_namespace) == true)
        return false;

    if (BARN_STR_CMP(id_namespace, "true") || BARN_STR_CMP(id_namespace, "false"))
        return false;

    return true;
}
   
void
barn_parse_access_element_structure(barn_parser_t* parser, barn_parser_access_element_t* access)
{
    if (!barn_is_type_struct(access->variable.variable->var_type->type))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of \".\" because \"%s\" is not a structure type", 
                        access->variable.variable->var_name);
    barn_append_element_to_array(access->access_struct.fields_of_struct, 
                                 access->variable.variable->var_name);
    barn_type_t* current_type = access->variable.variable->var_type;
    barn_parser_skip(parser, 1);

    while (parser->curr_token->kind == BARN_TOKEN_DOT)
    {
        if (parser->curr_token->kind == BARN_TOKEN_DOT)
            barn_parser_skip(parser, 1);

        const char* field_name = parser->curr_token->value;

        if (!barn_is_type_struct(current_type->type))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "couldn't find field named \"%s\" in \"%s\" type because it's not a structure", 
                            field_name, barn_convert_type_to_string(current_type));

        if (!barn_parser_is_structure_field_named(parser, current_type, field_name))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "couldn't find field named \"%s\" in \"%s\" type", 
                            field_name, current_type->structure.sturct_type_name);

        barn_append_element_to_array(access->access_struct.fields_of_struct, 
                                     parser->curr_token->value);
        barn_struct_field_t* field = barn_parser_get_structure_field_named(parser, current_type, field_name);
        current_type = field->field_type;
        barn_parser_skip(parser, 1);
    }

    access->access_struct.parent_variable_name = access->variable.variable->var_name;
    access->access_struct.parent_variable      = access->variable.variable;
    access->access_struct.struct_field_type    = current_type;

    access->is_access_struct = true;
    access->is_variable      = true;

    access->access_struct.parent_variable->is_used = true;
    barn_parser_skip(parser, -1);
}

barn_parser_access_element_t*
barn_parse_access_element(barn_parser_t* parser)
{
    barn_parser_access_element_t* access   = (barn_parser_access_element_t*)calloc(1, sizeof(barn_parser_access_element_t));
    access->access_struct.fields_of_struct = barn_create_array(sizeof(char*));

    // If a variable named curr_token exists and next token isn't a DOT "." then it's just 
    // a simple variable access
    if (((access->variable.variable = barn_parser_get_variable_by_name(parser, parser->curr_token->value)) != NULL) &&
        barn_parser_is_next_token(parser, BARN_TOKEN_DOT) == false)
    {
        access->variable.variable_name = parser->curr_token->value;
        access->is_variable = true;
        return access;
    }

    // If a variable named curr_token exists and next token is a DOT "." then program
    // is trying to access a structure of a variable 
    if (((access->variable.variable = barn_parser_get_variable_by_name(parser, parser->curr_token->value)) != NULL) &&
        barn_parser_is_next_token(parser, BARN_TOKEN_DOT) == true)
    {
        barn_parse_access_element_structure(parser, access);
        return access;
    }

    if ((barn_parser_function_get_by_name(parser, parser->curr_token->value) != NULL) &&
        barn_parser_is_next_token(parser, BARN_TOKEN_OPENPARENT) == true)
    {
        access->function.function_name = parser->curr_token->value;
        access->is_function = true;
        return access;
    }

    BARN_PARSER_ERR(parser, BARN_UNDEFINED_ERROR, "\"%s\" is undefined", parser->curr_token->value);
}

void
barn_parser_identifier(barn_parser_t* parser)
{
    if (!barn_parser_is_id_keyword(parser->curr_token->value))
    {
        barn_parser_access_element_t* element = barn_parse_access_element(parser);

        if (barn_parser_is_next_token(parser, BARN_TOKEN_OPENPARENT))
            barn_parser_func_call(parser, false, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_ASN))
            barn_parser_variable_asn(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_PLUSASN))
            barn_parser_variable_plusasn(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_MODASN))
            barn_parser_variable_modasn(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_MINUSASN))
            barn_parser_variable_minusasn(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_MULASN))
            barn_parser_variable_mulasn(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_DIVASN))
            barn_parser_variable_divasn(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_INCREMENTATION))
            barn_parser_variable_incrementation(parser, element);
        else if (barn_parser_is_next_token(parser, BARN_TOKEN_DECREMENTATION))
            barn_parser_variable_decrementation(parser, element);
        else
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of \"%s\"", parser->curr_token->value);

        return;
    }

    if      (BARN_TOKEN_CMP("fun"))
        barn_parser_function_declaration(parser);
    else if (BARN_TOKEN_CMP("return"))
        barn_parser_func_return(parser);
    else if (BARN_TOKEN_CMP("let") || BARN_TOKEN_CMP("const"))
        barn_parser_variable_declaration(parser, BARN_TOKEN_CMP("const"), false);
    else if (BARN_TOKEN_CMP("static"))
        barn_parser_static_variable_declaration(parser);
    else if (BARN_TOKEN_CMP("extern"))
        barn_parser_extern_function_declaration(parser);
    else if (BARN_TOKEN_CMP("if"))
        barn_parser_if_statement(parser);
    else if (BARN_TOKEN_CMP("elif"))
        barn_parser_elif_statement(parser);
    else if (BARN_TOKEN_CMP("else"))
        barn_parser_else_statement(parser);
    else if (BARN_TOKEN_CMP("while"))
        barn_parser_while_loop(parser);
    else if (BARN_TOKEN_CMP("for"))
        barn_parser_for_loop(parser);
    else if (BARN_TOKEN_CMP("break"))
        barn_parser_break_loop(parser);
    else if (BARN_TOKEN_CMP("continue"))
        barn_parser_continue_loop(parser);
    else if (BARN_TOKEN_CMP("enum"))
        barn_parser_enum(parser);
    else if (BARN_TOKEN_CMP("@import_c"))
        barn_parser_import_c(parser);
    else if (BARN_TOKEN_CMP("struct"))
        barn_parser_struct(parser);
    else
    {
        barn_token_print(parser->curr_token);
        BARN_UNIMPLEMENTED("this keyword is not implemented");
    }
}

void 
barn_parser_reset_local_variables(barn_parser_t* parser)
{
    barn_destroy_array(parser->local_variables);
    parser->local_variables = barn_create_array(sizeof(barn_node_t));
}

bool 
barn_parser_is_statement_opened(barn_parser_t* parser)
{
    return (parser->statement_open >= 1);
}

void
barn_parser_check_usage_of_local_variables(barn_parser_t* parser)
{
    BARN_ARRAY_FOR(parser->local_variables)
    {
        barn_variable_t* local_var = barn_get_element_from_array(parser->local_variables, i);
        if (local_var->is_used == false && strlen(local_var->var_name) != 0)
            barn_warning_show_line("[disable with --w-disable-unused] variable named \"%s\" is not used", local_var->var_token, local_var->var_name);            
    }
}

void 
barn_parser_close_brace(barn_parser_t* parser)
{
    if (!barn_parser_is_function_opened(parser))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unexpected use of '}'", 0);

    if (!barn_parser_is_statement_opened(parser))
    {
        parser->actual_function = NULL;
        barn_parser_check_usage_of_local_variables(parser);
        barn_parser_reset_local_variables(parser);
        parser->layer = -1;
        return;
    }

    barn_condition_statement_t kind_of_conf = -1;
    if (parser->statement_node->node_kind == BARN_NODE_CONDITION_STATEMENT)
        kind_of_conf = parser->statement_node->condition_statement.kind_of_cond;
    else if (parser->statement_node->node_kind == BARN_NODE_WHILE_LOOP)
        kind_of_conf = BARN_WHILE_LOOP;
    else
        BARN_UNIMPLEMENTED("u probably implemented for loop and forgot 'bout it..");

    barn_node_t* end_node = barn_create_empty_node(BARN_NODE_END_STATEMENT);
    end_node->end_statement.kind_of_cond = kind_of_conf;
    barn_parser_append_node(parser, end_node);
    
    parser->statement_open--;
    parser->layer--;
}

void
barn_parser_check_usage_of_global_variables(barn_parser_t* parser)
{
    if (barn_is_flag(parser->lexer->args_parser, "--w-disable-unused", "-wd-unused"))
        return;

    BARN_ARRAY_FOR(parser->global_variables)
    {
        barn_variable_t* global_var = barn_get_element_from_array(parser->global_variables, i);
        if (global_var->is_used == false && strlen(global_var->var_name) != 0)
            barn_warning_show("[disable with --w-disable-unused] variable named \"%s\" is not used", global_var->var_token, global_var->var_name);            
    }
}

void
barn_parser_check_is_main_function_defined(barn_parser_t* parser)
{
    if (barn_is_flag(parser->lexer->args_parser, "--no-main", "-nm"))
        return;

    if (barn_parser_function_get_by_name(parser, "main") == NULL)
    {
        barn_error_show(BARN_COMPILER_ERROR, "[disable with --no-main] \"main\" function is not defined");
        exit(EXIT_FAILURE);
    }
}

void 
barn_parser_main_loop(barn_parser_t* parser)
{
    for (; parser->index < parser->lexer->tokens->length; parser->index++)
    {
        barn_parser_skip(parser, 0);

        if (parser->curr_token->kind == BARN_TOKEN_EOF)
            break;
        else if (parser->curr_token->kind == BARN_TOKEN_NEWLINE)
            continue;
        else if (parser->curr_token->kind == BARN_TOKEN_IDENTIFIER)
            barn_parser_identifier(parser);
        else if (parser->curr_token->kind == BARN_TOKEN_CLOSEBRACE)
            barn_parser_close_brace(parser);
        else 
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "unknown use of '%s' token in this place", 
                            parser->curr_token->value);
    }

    barn_parser_check_usage_of_global_variables(parser);
    barn_parser_check_is_main_function_defined(parser);
}

barn_parser_t* 
barn_start_parser(barn_lexer_t* lexer)
{
    barn_parser_t* parser = (barn_parser_t*)calloc(1, sizeof(barn_parser_t));
    
    parser->global_variables = barn_create_array(sizeof(barn_variable_t));
    parser->local_variables  = barn_create_array(sizeof(barn_variable_t));
    parser->structure_nodes  = barn_create_array(sizeof(barn_node_t));
    parser->function_nodes   = barn_create_array(sizeof(barn_node_t));
    parser->nodes            = barn_create_array(sizeof(barn_node_t));

    parser->curr_token = NULL;
    parser->lexer      = lexer;

    parser->statement_open = 0;
    parser->index          = 0;

    parser->actual_function = NULL;

    barn_debug_entry("barn_parser_main_loop", __FILE__, __LINE__);
    barn_initialize_builtin_functions(parser);
    barn_parser_main_loop(parser);

    if (barn_is_flag(parser->lexer->args_parser, "--ast", "-a"))
        // barn_parser_show_ast(parser);
    return parser;
}

void
barn_parser_show_ast_child(barn_parser_t* parser, barn_array_t* nodes, int tabs)
{
    for (int i = 0; i < nodes->length; i++)
    {
        barn_node_t* curr_node = barn_get_element_from_array(nodes, i);

        barn_generate_tabs(tabs);
        printf("parser->nodes[%d]={ node_kind: %s, ", i, barn_node_kind_show(curr_node->node_kind));
        
        if (curr_node->node_kind == BARN_NODE_VARIABLE_DECLARATION)
            printf("variable_declaration: { var_name: \"%s\", var_type: \"%s\" }, ",
                curr_node->variable_declaration.variable->var_name,
                curr_node->variable_declaration.variable->var_type == NULL ? "auto/none" :
                    barn_convert_type_to_string(curr_node->variable_declaration.variable->var_type));

        if (curr_node->node_kind == BARN_NODE_FUNCTION_CALL)
            printf("function_call: { func_name: \"%s\", func_args->len: %lu }, ",
                curr_node->function_call.function_name,
                curr_node->function_call.function_args->length);

        if (curr_node->node_kind == BARN_NODE_END_STATEMENT)
            printf("end_statement: { condition_kind: \"%s\" }", 
                barn_condition_statement_string[curr_node->end_statement.kind_of_cond]);

        printf("}\n");
    }
}

void
barn_parser_show_ast(barn_parser_t* parser)
{
    printf("AST Tree debug:\n");

    for (int i = 0; i < parser->nodes->length; i++)
    {
        barn_node_t* curr_node = barn_get_element_from_array(parser->nodes, i);

        printf("parser->nodes[%d]={ node_kind: %s, ", i, barn_node_kind_show(curr_node->node_kind));

        if (curr_node->node_kind == BARN_NODE_FUNCTION_DECLARATION)
            printf("function_declaration: { function_name: \"%s\", function_nodes: %p }, ",
                curr_node->function_declaration.function_name,
                curr_node->function_declaration.function_nodes);

        if (curr_node->node_kind == BARN_NODE_VARIABLE_DECLARATION)
            printf("variable_declaration: { var_name: \"%s\", var_type: \"%s\" }, ",
                curr_node->variable_declaration.variable->var_name,
                curr_node->variable_declaration.variable->var_type == NULL ? "auto/none" :
                    barn_convert_type_to_string(curr_node->variable_declaration.variable->var_type));

        printf("}\n");

        if (curr_node->node_kind == BARN_NODE_FUNCTION_DECLARATION)
        {
            printf("{\n");
            barn_parser_show_ast_child(parser, curr_node->function_declaration.function_nodes, 1);
            printf("}\n");
        }
    }
}

bool 
barn_parser_is_function_opened(barn_parser_t* parser)
{
    return (parser->actual_function == NULL)
        ? false 
        : true;
}

void 
barn_parser_append_node(barn_parser_t* parser, barn_node_t* node)
{
    if (barn_parser_is_function_opened(parser))
    {
        barn_append_element_to_array(parser->actual_function->function_declaration.function_nodes,
                                     node);
        return;
    }

    barn_append_element_to_array(parser->nodes, node);
}

void
barn_parser_import_c(barn_parser_t* parser)
{
    if (!barn_parser_is_next_token(parser, BARN_TOKEN_STRING))
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "expected string after @import_c", 0);

    if (strlen(parser->curr_token->value) == 0)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "c header is too short", 0);
    barn_parser_skip(parser, 1);
    
    barn_node_t* node     = barn_create_empty_node(BARN_NODE_IMPORT_C);
    node->import_c.header = parser->curr_token->value;
    barn_parser_append_node(parser, node);
}