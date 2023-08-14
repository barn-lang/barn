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

#include <barn_string.h>

char* 
barn_duplicate_string(char* str)
{
    char* dup_string = (char*)malloc(sizeof(char) * strlen(str));

    BARN_NO_NULL(dup_string);
    strcpy(dup_string, str);

    return dup_string;
}

void
barn_append_char_to_allocated_string(char* str, char c)
{
    size_t str_length = strlen(str);
    str = (char*)realloc(str, str_length + 2);

    BARN_NO_NULL(str);

    str[str_length + 0] = c;
    str[str_length + 1] = '\0';
}