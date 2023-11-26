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

void 
barn_debug_success(const char* msg, const char* filename, int line)
{
#ifdef __BARN_SHOW_DEBUG__
    printf("\x1b[0;32mSUCCESS:\x1b[0;0m ");
    printf("%s in %d: ", filename, line);
    printf("%s\n", msg);
#endif /* __BARN_SHOW_DEBUG__ */
}

void 
barn_debug_error(const char* msg, const char* filename, int line)
{
#ifdef __BARN_SHOW_DEBUG__
    printf("\x1b[0;31mERROR:\x1b[0;0m ");
    printf("%s in %d: ", filename, line);
    printf("%s\n", msg);
#endif /* __BARN_SHOW_DEBUG__ */
}

void 
barn_debug_entry(const char* func, const char* filename, int line)
{
#ifdef __BARN_SHOW_DEBUG__
    printf("\x1b[0;34mENTRY:\x1b[0;0m ");
    printf("%s in %d: ", filename, line);
    printf("%s\n", func);
#endif /* __BARN_SHOW_DEBUG__ */
}