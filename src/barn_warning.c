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

#include <barn_colors.h>
#include <barn_debug.h>
#include <barn_warning.h>
#include <barn_lexer.h>

void
barn_warning_show(char* message, ...)
{
  va_list list;
  va_start(list, message);

  char buf[512];
  vsnprintf(buf, 512, message, list);

  printf("%swarning%s: %s\n\n",
  barn_get_bold_color_as_str_code(BARN_COLOR_YELLOW),
  barn_get_color_as_str_code(BARN_COLOR_RESET),
  buf);

  va_end(list);
}