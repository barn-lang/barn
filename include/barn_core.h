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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#include <sys/time.h>

#include <barn_debug.h>
#include <barn_error.h>

#ifndef __BARN_SHOW_DEBUG__
// # define __BARN_SHOW_DEBUG__ (0x0001)
#endif /* __BARN_SHOW_DEBUG__ */

#ifdef __BARN_SHOW_DEBUG__
# define BARN_CALL(func) ({                             \
    __typeof__(func) tmp = func;                        \
     barn_debug_entry(#func, __FILE_NAME__, __LINE__);  \
    &tmp;                                               \
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

#define BARN_UNIMPLEMENTED(msg) ({                                                          \
    barn_error_show(BARN_COMPILER_ERROR, "unimplemented feauter in %s in \x1b[1;37m%s\x1b[0;0m at %d: \x1b[1;31m%s\x1b[0;0m\n\t\tplease create a new issue on github that will report this weird action",\
        __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, msg);                                 \
    exit(1);                                                                                \
})        

#define BARN_STR_CMP(s1, s2) (strcmp(s1, s2) == 0)                                                                  

typedef char barn_error_or_t;

#define BARN_TRY(func) ({                                                          \
    barn_error_or_t ret = func;                                                    \
    if (ret == -1)                                                                 \
    {                                                                              \
        barn_error_show(BARN_COMPILER_ERROR, "function: %s, returned error (-1)",  \
            #func, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);                  \
        exit(1);                                                                   \
    }                                                                              \
})                                                                                 \

#define BARN_USE(x) ({ (void*)(x); })

const char* barn_generate_tabs_to_buf(int tabs);
void barn_generate_tabs(int tabs);

#endif /* __BARN_CORE__ */