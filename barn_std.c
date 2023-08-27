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
#include <barn_std.h>

#include <barn_string.h>

#ifndef BARN_STD_FOLDER
# define BARN_STD_FOLDER ".barn/libs/"
#endif /* BARN_STD_FOLDER */

// TODO: make this function more lightweight without each time using
//       sprintf
const char* 
barn_std_get_path()
{
#ifdef _WIN32
    return BARN_STD_FOLDER;
#else
    char* env_home = getenv("HOME");
    char* buffer = (char*)calloc(strlen(env_home) + strlen(BARN_STD_FOLDER) + 1, sizeof(char));
    sprintf(buffer, "%s/%s", env_home, BARN_STD_FOLDER);

    return barn_duplicate_string(buffer);
#endif /* _WIN32 */
}