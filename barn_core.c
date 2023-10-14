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

void
barn_generate_tabs(int tabs)
{
    for (int i = 0; i < tabs; i++)
        printf("\t");
}

const char*
barn_generate_tabs_to_buf(int tabs)
{
    char* buf = barn_create_allocated_string();

    for (int i = 0; i < tabs; i++)
        barn_append_char_to_allocated_string(&buf, '\t');

    return ((const char*)buf);
}