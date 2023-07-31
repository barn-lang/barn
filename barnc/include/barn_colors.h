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

#ifndef __BARN_COLORS__
#define __BARN_COLORS__

#include <barn_core.h>

typedef enum __barn_colors_t {
    BARN_COLOR_BLACK = 30,		
    BARN_COLOR_RED,
    BARN_COLOR_GREEN,
    BARN_COLOR_YELLOW,
    BARN_COLOR_BLUE,
    BARN_COLOR_MAGENTA,
    BARN_COLOR_CYAN,
    BARN_COLOR_WHITE,
    BARN_COLOR_GRAY = 90,
    BARN_COLOR_BRIGHT_GRAY = 97,
    BARN_COLOR_RESET = 0,
} barn_colors_t;

char* barn_get_color_as_str_code(barn_colors_t color);

char* barn_get_bold_color_as_str_code(barn_colors_t color);

#endif /* __BARN_COLORS__ */