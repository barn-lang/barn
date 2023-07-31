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

#ifndef __BARN_ARGS_PARSER__
#define __BARN_ARGS_PARSER__

#include <barn_core.h>
#include <barn_array.h>

typedef struct __barn_args_parser_t {
    char** argv;
    int argc;

    bool is_filename;
    char* filename;
    
    barn_array_t* flags;
} barn_args_parser_t;

barn_args_parser_t* barn_args_parser_start(int argc, char** argv);

char* barn_get_flag_by_index(barn_args_parser_t* args_parser, int index);

int barn_get_flag_index(barn_args_parser_t* args_parser, char* flag);

bool barn_is_flag(barn_args_parser_t* args_parser, char* flag);

#endif /* __BARN_ARGS_PARSER__ */