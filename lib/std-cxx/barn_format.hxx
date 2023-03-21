/*
 *   Copyright (C) 2022-2023 Barn-Lang Organization
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

#ifndef __BARN_FORMAT_HXX__
#define __BARN_FORMAT_HXX__

#include <stdarg.h>

__builtin_va_list __barn_va_arg__;

/* Start and end format */
#define __barn_start_format() {                                    \
            __builtin_va_start(__barn_va_arg__, __format_start__); \
};

#define __barn_end_format() {                   \
            __builtin_va_end(__barn_va_arg__);  \
};

/* Get values */
#define __barn_format_get_value_string() {                 \
            __builtin_va_arg(__barn_va_arg__, std::string)  \
};

#define __barn_format_get_value_cstr() {                   \
            __builtin_va_arg(__barn_va_arg__, char*)        \
};

#define __barn_format_get_value_int()    {         \
            __builtin_va_arg(__barn_va_arg__, int) \
};

#define __barn_format_get_value_long()    {         \
            __builtin_va_arg(__barn_va_arg__, long) \
};

#define __barn_format_get_value_double()  {           \
            __builtin_va_arg(__barn_va_arg__, double) \
};

#endif /* __BARN_FORMAT_HXX__ */