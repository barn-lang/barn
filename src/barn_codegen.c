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

#include <barn_codegen.h>

#include <barn_core.h>
#include <barn_parser.h>
#include <barn_string.h>
#include <barn_functions.h>
#include <barn_std.h>
#include <barn_enum.h>

#include <config/barn_config.h>

#ifdef BARN_CODEGEN_PRETTIER_CODE
# define BARN_CODEGEN_GENERATE_TABS(codegen)                                         \
     ({ fprintf(codegen->c_file, "%s", barn_generate_tabs_to_buf(codegen->tabs)); }) 
#else
# define BARN_CODEGEN_GENERATE_TABS(codegen) ("")
#endif /* BARN_CODEGEN_PRETTIER_CODE */

barn_codegen_t*
barn_codegen_create(barn_parser_t* parser)
{
    barn_codegen_t* codegen = (barn_codegen_t*)calloc(1, sizeof(barn_codegen_t));
    BARN_NO_NULL(codegen);

    codegen->parser = parser;
    codegen->tabs   = 0;

    codegen->c_file = fopen("barn.c", "w+");

    return codegen;
}

void
barn_codegen_add_header(barn_codegen_t* codegen, const char* c_header)
{
    fprintf(codegen->c_file, "#include \"");
    fprintf(codegen->c_file, "%s", barn_std_get_path());
    fprintf(codegen->c_file, "%s", c_header);
#ifdef BARN_CODEGEN_PRETTIER_CODE
    fprintf(codegen->c_file, "\"\n\n");
#else
    fprintf(codegen->c_file, "\"\n");
#endif
}

char*
barn_codegen_operator_to_code(barn_token_kind_t op)
{
    switch (op)
    {
        case BARN_TOKEN_PLUS:
            return "+";
            break;
        case BARN_TOKEN_MINUS:
            return "-";
            break;
        case BARN_TOKEN_MUL:
            return "*";
            break;
        case BARN_TOKEN_DIV:
            return "/";
            break;
        case BARN_TOKEN_GT: 
            return ">";
            break;
        case BARN_TOKEN_GTE:
            return ">=";
            break;
        case BARN_TOKEN_LT:
            return "<";
            break;
        case BARN_TOKEN_LTE:
            return "<=";
            break;
        case BARN_TOKEN_EQ:
            return "==";
            break;
        case BARN_TOKEN_NEQ:
            return "!=";
            break;
        case BARN_TOKEN_OROR:
            return "||";
            break;
        case BARN_TOKEN_ANDAND:
            return "&&";
            break;
        case BARN_TOKEN_MOD:
            return "%";
            break;
        default:
            BARN_UNIMPLEMENTED("unknown operator to code generate");
            break;
    }

    return "unknown";
}

const char* 
barn_codegen_expression_generate(barn_codegen_t* codegen, barn_node_t* expression_node)
{
    barn_array_t* expression_arr = expression_node->expression.expression_nodes;

    char* expression_buf = barn_create_allocated_string();
    int   last_parent    = 0;

    BARN_ARRAY_FOR(expression_arr)
    {
        barn_expression_node_t* curr_expr_node = barn_get_element_from_array(expression_arr, i);

        if (last_parent != curr_expr_node->parents)
        {
            if (last_parent > curr_expr_node->parents)
            {
                for (int j = 0; j < last_parent - curr_expr_node->parents; j++)
                    barn_append_char_to_allocated_string(&expression_buf, ')');
            }
            else if (last_parent < curr_expr_node->parents) 
            {
                for (int j = 0; j < curr_expr_node->parents - last_parent; j++)
                    barn_append_char_to_allocated_string(&expression_buf, '(');
            }

            last_parent = curr_expr_node->parents;
        }

        if (curr_expr_node->lhs != NULL && curr_expr_node->lhs->expr_val_token != NULL)
        {
            if (curr_expr_node->lhs->is_function_call == true)
                barn_append_string_to_allocated_string(&expression_buf, 
                    barn_codegen_function_call(codegen, curr_expr_node->lhs->function_call, false, false));
            else if (curr_expr_node->lhs->is_variable == true)
                barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->lhs->expr_val_token->value);
            else
            {
                if (curr_expr_node->lhs->expr_val_type->is_string)
                {
                    barn_append_char_to_allocated_string(  &expression_buf, '\"');
                    barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->lhs->expr_val_token->value);
                    barn_append_char_to_allocated_string(  &expression_buf, '\"');
                }
                else if (curr_expr_node->lhs->expr_val_type->is_char)
                {
                    barn_append_char_to_allocated_string(  &expression_buf, '\'');
                    barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->lhs->expr_val_token->value);
                    barn_append_char_to_allocated_string(  &expression_buf, '\'');
                }
                else
                {
                    barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->lhs->expr_val_token->value);
                }
            }
        }

        if (curr_expr_node->operator != BARN_TOKEN_NONE)
        {
#ifdef BARN_CODEGEN_PRETTIER_CODE
            barn_append_char_to_allocated_string(  &expression_buf, ' ');
#endif /* BARN_CODEGEN_PRETTIER_CODE */
            barn_append_string_to_allocated_string(&expression_buf, barn_codegen_operator_to_code(curr_expr_node->operator));
#ifdef BARN_CODEGEN_PRETTIER_CODE
            barn_append_char_to_allocated_string(  &expression_buf, ' ');
#endif /* BARN_CODEGEN_PRETTIER_CODE */
        }

        if (curr_expr_node->rhs != NULL && curr_expr_node->rhs->expr_val_token != NULL)
        {
            if (curr_expr_node->rhs->is_function_call == true)
                barn_append_string_to_allocated_string(&expression_buf, 
                    barn_codegen_function_call(codegen, curr_expr_node->rhs->function_call, false, false));
            else if (curr_expr_node->rhs->is_variable == true)
                barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->rhs->expr_val_token->value);
            else
            {
                if (curr_expr_node->rhs->expr_val_type->is_string)
                {
                    barn_append_char_to_allocated_string(  &expression_buf, '\"');
                    barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->rhs->expr_val_token->value);
                    barn_append_char_to_allocated_string(  &expression_buf, '\"');
                }
                else if (curr_expr_node->rhs->expr_val_type->is_char)
                {
                    barn_append_char_to_allocated_string(  &expression_buf, '\'');
                    barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->rhs->expr_val_token->value);
                    barn_append_char_to_allocated_string(  &expression_buf, '\'');
                }
                else
                    barn_append_string_to_allocated_string(&expression_buf, curr_expr_node->rhs->expr_val_token->value);
            }
        }
    }

    for (int i = 0; i < last_parent; i++)
    {
        barn_append_char_to_allocated_string(&expression_buf, ')');
    }

    return ((const char*)expression_buf);
}

void
barn_codegen_function_return(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    BARN_CODEGEN_GENERATE_TABS(codegen);

    const char* expr_code = barn_codegen_expression_generate(codegen, curr_node->function_return.return_value);
    fprintf(codegen->c_file, "return %s;", expr_code);
}

const char*
barn_codegen_function_call(barn_codegen_t* codegen, barn_node_t* curr_node, bool colon, bool tabs)
{
    char* buf = barn_create_allocated_string();

    if (tabs)
        BARN_CODEGEN_GENERATE_TABS(codegen);

    if (strcmp(curr_node->function_call.function_name, BARN_FUNCTION_INJECTING_CODE) == 0)
    {
        barn_node_t* argument_expr = barn_get_element_from_array(curr_node->function_call.function_args, 0);
        char* expr_code = (char*)barn_codegen_expression_generate(codegen, argument_expr);
        
        *expr_code++;
        expr_code[strlen(expr_code) - 1] = '\0';
        
        barn_append_string_to_allocated_string(&buf, expr_code);
        return buf;
    }

    barn_append_string_to_allocated_string(&buf, curr_node->function_call.function_name);
    barn_append_char_to_allocated_string(&buf, '(');

    BARN_ARRAY_FOR(curr_node->function_call.function_args)
    {
        barn_node_t* argument_expr = barn_get_element_from_array(curr_node->function_call.function_args, i);

        const char* expr_code = barn_codegen_expression_generate(codegen, argument_expr);

        barn_append_string_to_allocated_string(&buf, expr_code);

        if (curr_node->function_call.function_args->length != (i + 1))
            barn_append_char_to_allocated_string(&buf, ',');
    }

    barn_append_char_to_allocated_string(&buf, ')');

    if (colon)
        barn_append_char_to_allocated_string(&buf, ';');

    return buf;
}

void
barn_codegen_enum(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    BARN_CODEGEN_GENERATE_TABS(codegen);

    fprintf(codegen->c_file, "enum {\n");
    BARN_ARRAY_FOR(curr_node->enumerate.enum_fields)
    {
        const barn_enum_field_t* field = barn_get_element_from_array(curr_node->enumerate.enum_fields, i);

        fprintf(codegen->c_file, "%s", field->enum_name);

        if (field->enum_expression != NULL)
            fprintf(codegen->c_file, " = %s", 
                barn_codegen_expression_generate(codegen, field->enum_expression));

        if (!BARN_ARRAY_IS_LAST_ELEMENT(curr_node->enumerate.enum_fields, i))
            fprintf(codegen->c_file, ",");
        fprintf(codegen->c_file, "\n");
    }
    fprintf(codegen->c_file, "};");
}

void
barn_codegen_variable_declaration(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    BARN_CODEGEN_GENERATE_TABS(codegen);

    if (curr_node->variable_declaration.variable->is_static)
        fprintf(codegen->c_file, "static ");

    if (curr_node->variable_declaration.variable->is_const)
        fprintf(codegen->c_file, "const ");

    fprintf(codegen->c_file, "%s %s = ", 
        barn_codegen_type_convert_to_c(codegen, curr_node->variable_declaration.variable->var_type),
        curr_node->variable_declaration.variable->var_name);

    const char* variable_value = barn_codegen_expression_generate(codegen, curr_node->variable_declaration.variable_value);

    fprintf(codegen->c_file, "%s", variable_value);
    fprintf(codegen->c_file, ";");
}

void
barn_codegen_condition_statement(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    BARN_CODEGEN_GENERATE_TABS(codegen);
    codegen->tabs++;

    char* condition_keyword = NULL;
    if (curr_node->condition_statement.kind_of_cond == BARN_IF_CONDITION)
        condition_keyword = "if";
    else if (curr_node->condition_statement.kind_of_cond == BARN_ELIF_CONDITION)
        condition_keyword = "else if";

    if (curr_node->condition_statement.kind_of_cond == BARN_ELSE_CONDITION)
        fprintf(codegen->c_file, "else {");
    else
        fprintf(codegen->c_file, "%s (%s) {",
            condition_keyword,
            barn_codegen_expression_generate(codegen, curr_node->condition_statement.condition_expr));
}

void
barn_codegen_while_loop(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    BARN_CODEGEN_GENERATE_TABS(codegen);
    codegen->tabs++;

    fprintf(codegen->c_file, "while (%s) {",
        barn_codegen_expression_generate(codegen, curr_node->while_loop.condition_expr));
}

void
barn_codegen_variable_modification(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    BARN_CODEGEN_GENERATE_TABS(codegen);
    
    const char* variable_name = curr_node->variable_modification.variable->var_name;
    fprintf(codegen->c_file, "%s ", variable_name);

    switch (curr_node->node_kind)
    {
        case BARN_NODE_VARIABLE_ASN:
            fprintf(codegen->c_file, "= ");
            break;
        case BARN_NODE_VARIABLE_ASNMINUS:
            fprintf(codegen->c_file, "-= ");
            break;
        case BARN_NODE_VARIABLE_ASNPLUS:
            fprintf(codegen->c_file, "+= ");
            break;
        case BARN_NODE_VARIABLE_ASNMUL:
            fprintf(codegen->c_file, "*= ");
            break;
        case BARN_NODE_VARIABLE_ASNDIV:
            fprintf(codegen->c_file, "/= ");
            break;
        case BARN_NODE_VARIABLE_INCREMENTATION:
            fprintf(codegen->c_file, "++;");
            break;
        case BARN_NODE_VARIABLE_DECREMENTATION:
            fprintf(codegen->c_file, "--;");
            break;
        default:
            printf("%s", barn_node_kind_show(curr_node->node_kind));
            BARN_UNIMPLEMENTED("unimplemented variable modification in codegen");
            break;
    }

    // This means that we are dealing with a variable modification node
    // that's doesn't use new value like incrementation or decrementation
    if (curr_node->variable_modification.variable_value == NULL)
        return;

    fprintf(codegen->c_file, "%s;",
        barn_codegen_expression_generate(codegen, curr_node->variable_modification.variable_value));
}

void
barn_codegen_generate_function_body(barn_codegen_t* codegen, barn_node_t* curr_node)
{
    switch (curr_node->node_kind)
    {
        case BARN_NODE_FUNCTION_DECLARATION:
            BARN_UNIMPLEMENTED("compiler error, function declaration can't happend inside a funciton");
            break;
        case BARN_NODE_FUNCTION_RETURN:
            barn_codegen_function_return(codegen, curr_node);
            break;
        case BARN_NODE_FUNCTION_CALL:
            fprintf(codegen->c_file, "%s", barn_codegen_function_call(codegen, curr_node, true, true));
            break;
        case BARN_NODE_VARIABLE_DECLARATION:
            barn_codegen_variable_declaration(codegen, curr_node);
            break;
        case BARN_NODE_VARIABLE_ASN:
        case BARN_NODE_VARIABLE_ASNDIV:
        case BARN_NODE_VARIABLE_ASNMINUS:
        case BARN_NODE_VARIABLE_ASNPLUS:
        case BARN_NODE_VARIABLE_ASNMUL:
        case BARN_NODE_VARIABLE_INCREMENTATION:
        case BARN_NODE_VARIABLE_DECREMENTATION:
            barn_codegen_variable_modification(codegen, curr_node);
            break;
        case BARN_NODE_CONDITION_STATEMENT:
            barn_codegen_condition_statement(codegen, curr_node);
            break;
        case BARN_NODE_WHILE_LOOP:
            barn_codegen_while_loop(codegen, curr_node);
            break;
        case BARN_NODE_END_STATEMENT:
            codegen->tabs--;
            BARN_CODEGEN_GENERATE_TABS(codegen);
            fprintf(codegen->c_file, "}");
            break;
        case BARN_NODE_BREAK_LOOP:
        case BARN_NODE_CONTINUE_LOOP:
            BARN_CODEGEN_GENERATE_TABS(codegen);
            fprintf(codegen->c_file, "%s;", (curr_node->node_kind == BARN_NODE_BREAK_LOOP 
                                                ? "break" 
                                                : "continue"));
            break;
        case BARN_NODE_IMPORT_C:
            if (barn_string_prefix(codegen->curr_node->import_c.header, "./"))
                fprintf(codegen->c_file, "#include \"%s\"\n\n", codegen->curr_node->import_c.header);
            else
                fprintf(codegen->c_file, "#include <%s>\n\n", codegen->curr_node->import_c.header);
            break;
        default:
            printf("unimplemented node kind -> %s\n", barn_node_kind_show(curr_node->node_kind));
            BARN_UNIMPLEMENTED("generating code for given node kind in function body is unimplemented feauter");
            break;
    }
}

void 
barn_codegen_skip(barn_codegen_t* codegen, int count)
{
    codegen->index += count;
    codegen->curr_node = barn_get_element_from_array(codegen->parser->nodes, codegen->index);
}

void
barn_codegen_function_declaration(barn_codegen_t* codegen)
{
    barn_node_t* fn_decl = codegen->curr_node;

    if (fn_decl->function_declaration.function_extern)
        fprintf(codegen->c_file, "extern ");

    fprintf(codegen->c_file, "%s %s(",
        barn_codegen_type_convert_to_c(codegen, fn_decl->function_declaration.function_return),
        fn_decl->function_declaration.function_name);

    BARN_ARRAY_FOR(fn_decl->function_declaration.function_args)
    {
        barn_func_argument_t* argument = barn_get_element_from_array(
                                            fn_decl->function_declaration.function_args, i);
        
        const char* ctype = barn_codegen_type_convert_to_c(codegen, argument->argument_type);

        if (BARN_ARRAY_IS_LAST_ELEMENT(fn_decl->function_declaration.function_args, i))
            fprintf(codegen->c_file, "%s %s",
                ctype, argument->argument_name);
        else
            fprintf(codegen->c_file, "%s %s, ",
                ctype, argument->argument_name);
    }

    if (!fn_decl->function_declaration.function_extern)
    {
        fprintf(codegen->c_file, ")\n{\n");
        codegen->tabs += 1;

        BARN_ARRAY_FOR(fn_decl->function_declaration.function_nodes)
        {
            barn_codegen_generate_function_body(codegen, 
                                                barn_get_element_from_array(fn_decl->function_declaration.function_nodes, i));

            if ((i + 1) != fn_decl->function_declaration.function_nodes->length)
                fprintf(codegen->c_file, "\n");
        }

        fprintf(codegen->c_file, "\n}\n\n");
        codegen->tabs -= 1;
    } else
        fprintf(codegen->c_file, ");\n\n");

}

barn_codegen_t* 
barn_codegen_start(barn_parser_t* parser)
{
    barn_codegen_t* codegen = barn_codegen_create(parser);

    // TODO: if flag --no-stdlib is on don't add barn_header.h
    if (!barn_is_flag(parser->lexer->args_parser, "--no-stdlib", "-n-stdlib"))
    {
        barn_codegen_add_header(codegen, "std-c/barn_format.h");
        barn_codegen_add_header(codegen, "std-c/barn_header.h");
    }

    for (codegen->index = 0; codegen->index < parser->nodes->length; codegen->index++)
    {
        barn_codegen_skip(codegen, 0);
        
        if (codegen->curr_node->node_kind == BARN_NODE_FUNCTION_DECLARATION)
            barn_codegen_function_declaration(codegen);
        else if (codegen->curr_node->node_kind == BARN_NODE_VARIABLE_DECLARATION)
        {
            barn_codegen_variable_declaration(codegen, codegen->curr_node);
            fprintf(codegen->c_file, "\n\n");
        }
        else if (codegen->curr_node->node_kind == BARN_NODE_ENUM)
        {
            barn_codegen_enum(codegen, codegen->curr_node);
            fprintf(codegen->c_file, "\n\n");
        }
        else if (codegen->curr_node->node_kind == BARN_NODE_IMPORT_C)
        {
            if (barn_string_prefix(codegen->curr_node->import_c.header, "./"))
                fprintf(codegen->c_file, "#include \"%s\"\n\n", codegen->curr_node->import_c.header);
            else
                fprintf(codegen->c_file, "#include <%s>\n\n", codegen->curr_node->import_c.header);
        }
        else if (codegen->curr_node->node_kind == BARN_NODE_FUNCTION_CALL)
        {
            barn_node_t* first_argument = barn_get_element_from_array(codegen->curr_node->function_call.function_args, 0);
            fprintf(codegen->c_file, "%s\n", barn_codegen_expression_generate(codegen, first_argument));
        }
        else 
            BARN_UNIMPLEMENTED("unhandled node kind");
    }

    fclose(codegen->c_file);
    return codegen;
}

const char* 
barn_codegen_type_convert_to_c(barn_codegen_t* codegen, barn_type_t* type)
{
    switch (type->type)
    {
        case BARN_TYPE_U8:
            return "unsigned char";
            break;
        case BARN_TYPE_I8:
            return "char";
            break;
        case BARN_TYPE_BOOL:
            return "bool";
            break;
        case BARN_TYPE_U16:
            return "unsigned short";
            break;
        case BARN_TYPE_I16:
            return "short";
            break;
        case BARN_TYPE_U32:
            return "unsigned int";
            break;
        case BARN_TYPE_I32:
            return "int";
            break;
        case BARN_TYPE_F32:
            return "float";
            break;
        case BARN_TYPE_U64:
            return "unsigned long";
            break;
        case BARN_TYPE_I64:
            return "long";
            break;
        case BARN_TYPE_F64:
            return "double";
            break;
        case BARN_TYPE_STRING:
            return "char*";
            break;
        case BARN_TYPE_NONE:
            return "void";
            break;
        case BARN_TYPE_FORMAT:
            return "...";
            break;
        default:
            BARN_UNIMPLEMENTED("unhandled type size");
            break;        
    }

    return "unknown";
}