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

#ifndef __BARN_CODEGEN__
#define __BARN_CODEGEN__

#include <barn_parser.h>

typedef struct __barn_codegen_t {
    barn_parser_t* parser;
    barn_node_t*   curr_node;

    int tabs;

    FILE* c_file;
} barn_codegen_t;

barn_codegen_t* barn_codegen_start(barn_parser_t* parser);

const char* barn_codegen_type_convert_to_c(barn_codegen_t* codegen);
const char* barn_codegen_save_output_to_file(barn_codegen_t* codegen, const char* filename);

#endif /* __BARN_CODEGEN__ */