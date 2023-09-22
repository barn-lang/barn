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

#include <barn_parser.h>
#include <barn_expressions.h>
#include <barn_functions.h>

void 
barn_parser_func_call(barn_parser_t* parser)
{
    const char* function_name = BARN_COLLECT_CURR_TOK_VAL(parser);

    if (!BARN_STR_CMP(function_name, BARN_FUNCTION_INJECTING_CODE))
        if (!barn_parser_is_function_opened(parser))
            BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function should be opened to call a function", 0);

    barn_node_t* mentioned_function = barn_parser_function_get_by_name(parser, function_name);

    if (mentioned_function == NULL)
        BARN_PARSER_ERR(parser, BARN_SYNTAX_ERROR, "function named \"%s\" doesn't exists", function_name);
        
}