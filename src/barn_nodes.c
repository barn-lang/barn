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

#include <barn_nodes.h>

char* 
barn_node_kind_show(barn_node_kind_t kind)
{
    switch (kind)
    {
        case BARN_NODE_FUNCTION_DECLARATION:
            return "FunctionDeclaration";
        case BARN_NODE_FUNCTION_RETURN:
            return "FunctionReturn";
        case BARN_NODE_FUNCTION_CALL:
            return "FunctionCall";
        case BARN_NODE_EXPRESSION:
            return "Expression";
        case BARN_NODE_VARIABLE_DECLARATION:
            return "VariableDeclaration";
        case BARN_NODE_VARIABLE_ASN:
            return "VariableAssignment";
        case BARN_NODE_VARIABLE_ASNDIV:
            return "VariableAssignmentDivide";
        case BARN_NODE_VARIABLE_ASNPLUS:
            return "VariableAssignmentPlus";
        case BARN_NODE_VARIABLE_ASNMINUS:
            return "VariableAssignmentMinus";
        case BARN_NODE_VARIABLE_ASNMUL:
            return "VariableAssignmentMultiply";
        case BARN_NODE_VARIABLE_INCREMENTATION:
            return "VariableIncrementation";
        case BARN_NODE_VARIABLE_DECREMENTATION:
            return "VariableDecrementation";
        case BARN_NODE_CONDITION_STATEMENT:
            return "ConditionStatement";
        case BARN_NODE_END_STATEMENT:
            return "EndStatement";
        case BARN_NODE_FOR_LOOP:
            return "ForLoop";
        case BARN_NODE_WHILE_LOOP:
            return "WhileLoop";
        case BARN_NODE_BREAK_LOOP:
            return "BreakLoop";
        case BARN_NODE_CONTINUE_LOOP:
            return "ContinueLoop";
        case BARN_NODE_ENUM:
            return "Enum";
        case BARN_NODE_IMPORT_C:
            return "ImportC";
        case BARN_NODE_STRUCT:
            return "Struct";
    }

    BARN_UNIMPLEMENTED("unhandled node type");
    return NULL;
}

void 
barn_node_show(barn_node_t* node)
{
    printf("{ node_kind: %s }\n",
        barn_node_kind_show(node->node_kind));
}

barn_node_t* 
barn_create_empty_node(barn_node_kind_t kind)
{
    barn_node_t* node = (barn_node_t*)calloc(1, sizeof(barn_node_t));
    node->node_kind   = kind;

    BARN_NO_NULL(node);
    return node;
}