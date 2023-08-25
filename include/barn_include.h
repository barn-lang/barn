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

#ifndef __BARN_INCLUDE__
#define __BARN_INCLUDE__

#include <barn_core.h>
#include <barn_lexer.h>

typedef struct __barn_include_t {
    barn_lexer_t* main_lexer;
    barn_token_t* curr_token;
    
    int index;
} barn_include_t;

void barn_include_files(barn_lexer_t* lexer);

#endif /* __BARN_INCLUDE__ */