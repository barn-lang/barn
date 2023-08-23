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

#ifndef __BARN_IO__
#define __BARN_IO__

#include <barn_core.h>
#include <barn_array.h>

bool barn_file_exists(const char* filename);

int64_t barn_get_file_size(FILE* f);

char* barn_read_whole_file(const char* filename);

char* barn_get_current_architecture();

int64_t barn_get_file_amount_of_lines(FILE* f);

#endif /* __BARN_IO__ */