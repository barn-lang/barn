/*
 *   Copyright (C) 2022-2024 Barn-Lang Organization
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __BARN_HEADER_h__
#define __BARN_HEADER_h__

#define __BARN_ENUM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

#if defined(__unix__) || defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <sys/utsname.h>
#elif defined(_WIN32)
    #include <windows.h>
#endif

#define __use__(x) ({(void)x;})

#ifdef __BARN_ENUM__
#else
# error "Enums are not enabled"
#endif

#endif /* __BARN_HEADER_h__ */