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

#ifndef __BARN_CORE__
#define __BARN_CORE__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include <barn_debug.h>
#include <barn_error.h>

#ifndef __BARN_SHOW_DEBUG__
# define __BARN_SHOW_DEBUG__ (0x0001)
#endif /* __BARN_SHOW_DEBUG__ */

#ifdef __BARN_SHOW_DEBUG__
# define BARN_CALL(func) ({                              \
     __typeof__(func) tmp = func;                        \
     barn_debug_entry(#func, __FILE_NAME__, __LINE__);   \
     tmp;                                                \
 })
#else
# define BARN_CALL(func) ({                          \
     func;                                           \
 })
#endif /* __BARN_SHOW_DEBUG__ */

#define BARN_NO_NULL(ptr) ({                                                                 \
    if (ptr == NULL)                                                                         \
    {                                                                                        \
        barn_error_show(BARN_COMPILER_ERROR, "%s is null. Happend in: %s in %s on line %d",  \
                        #ptr, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);                 \
        exit(1);                                                                             \
    }                                                                                        \
})

#endif /* __BARN_CORE__ */