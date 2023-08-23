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
    char* dup_string = (char*)calloc(strlen(str) + 1, sizeof(char));

    BARN_NO_NULL(dup_string);
    strcpy(dup_string, str);

    return dup_string;
}

char* 
barn_create_allocated_string()
{
    char* str = (char*)calloc(sizeof(char), 1);
    BARN_NO_NULL(str);

    str[0] = '\0';
    return str;
}

void
barn_append_char_to_allocated_string(char** str, char c)
{
    size_t old_length = *str ? strlen(*str) : 0; // Handle case where str is initially NULL
    size_t new_length = old_length + 2; // 1 for the new character and 1 for the null terminator

    // Reallocate memory for the new string
    char* new_str = (char*)realloc(*str, new_length * sizeof(char));
    if (new_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return; // Error handling
    }

    // Append the character and null-terminate the string
    new_str[old_length] = c;
    new_str[new_length - 1] = '\0';

    *str = new_str; // Update the pointer to the new string
}

char* 
barn_create_string_from_char(char c)
{
    char* str = (char*)calloc(2, sizeof(char));
    BARN_NO_NULL(str);

    str[0] = c;
    str[1] = 0;
    
    return str;
}