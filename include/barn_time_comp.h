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

#ifndef __BARN_TIME_COMP__
#define __BARN_TIME_COMP__

#include <barn_core.h>

#include <barn_args_parser.h>
#include <time.h>

typedef struct __barn_time_compilation_t {
    struct timeval start_time; 
    struct timeval end_time; 
} barn_time_compilation_t;

barn_time_compilation_t* barn_start_time_compilation(barn_args_parser_t* args_parser);

void barn_end_time_compilation(barn_time_compilation_t* time_comp);

void barn_print_time_compilation(barn_time_compilation_t* time_comp);

#endif /* __BARN_TIME_COMP__ */