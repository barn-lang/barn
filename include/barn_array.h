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

#ifndef __BARN_ARRAY__
#define __BARN_ARRAY__

#include <barn_core.h>

typedef struct __barn_array_t {
    size_t size_of_element;
    size_t length;

    void** ptr;
} barn_array_t;

barn_array_t* barn_create_array(size_t size_of_element);
barn_array_t* barn_destroy_array(barn_array_t* array);

void* barn_get_element_from_array(barn_array_t* array, size_t index);

/* Returns 0 on success -1 on error */
int barn_append_element_to_array(barn_array_t* array, void* element);

/* Returns 0 on success -1 on error */
int barn_delete_element_from_array(barn_array_t* array, size_t index);

#endif /* __BARN_ARRAY__ */