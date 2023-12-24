/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2024
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

#define BARN_ARRAY_FOR(arr) for (int i = 0; i < arr->length; i++)

#define BARN_ARRAY_IS_LAST_ELEMENT(arr, i) (arr->length == (i + 1))

barn_array_t* barn_create_array(size_t size_of_element);
barn_array_t* barn_destroy_array(barn_array_t* array);

void* barn_get_element_from_array(barn_array_t* array, size_t index);

barn_error_or_t barn_append_element_to_array(barn_array_t* array, void* element);
barn_error_or_t barn_delete_element_from_array(barn_array_t* array, size_t index);
barn_error_or_t barn_insert_element_in_array(barn_array_t* array, size_t index, void* element);
barn_error_or_t barn_delete_last_element_from_array(barn_array_t* array);
barn_error_or_t barn_resize_array(barn_array_t* array, size_t n);

#endif /* __BARN_ARRAY__ */