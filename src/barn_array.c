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

#include <barn_error.h>
#include <barn_string.h>
#include <barn_array.h>

#ifndef ARRAY_VALIDATE_NUMBER
# define ARRAY_VALIDATE_NUMBER(num) ({                                                                              \
     if (num < 0)                                                                                                   \
     {                                                                                                              \
         barn_error_show(BARN_COMPILER_ERROR, "ARRAY_VALIDATE_NUMBER() went wrong while creating/modifing array");  \
         exit(1);                                                                                                   \
     }                                                                                                              \
 })
#endif /* ARRAY_VALIDATE_NUMBER */

barn_array_t* 
barn_create_array(size_t size_of_element)
{
    barn_debug_entry("barn_create_array", __FILE_NAME__, __LINE__);

    ARRAY_VALIDATE_NUMBER(size_of_element);

    /* Initialize array fields with default 
     * values */
    barn_array_t* array = (barn_array_t*)calloc(1, sizeof(barn_array_t));
    BARN_NO_NULL(array);

    array->size_of_element = size_of_element;
    array->length          = 0;
    array->ptr             = (void**)calloc(array->length + 1, sizeof(void*));

    return array;
}

barn_array_t* 
barn_destroy_array(barn_array_t* array)
{
    if (array == NULL) return NULL;

    free(array->ptr);
    free(array);

    return NULL;
}

void* 
barn_get_element_from_array(barn_array_t* array, size_t index)
{
#ifdef __BARN_SHOW_DEBUG__

    if (index < 0)
    {
        barn_debug_error("barn_get_element_from_array: index need to be bigger than 0", __FILE_NAME__, __LINE__);
    }

#endif /* __BARN_SHOW_DEBUG__ */

    if (array == NULL || index >= array->length || index < 0)
        return NULL;

    return array->ptr[index];
}

barn_error_or_t 
barn_append_element_to_array(barn_array_t* array, void* element)
{
    if (array == NULL || element == NULL)
        return -1;

    void** new_ptr = (void**)realloc(array->ptr, (array->length + 1) * sizeof(void*));
    BARN_NO_NULL(new_ptr);

    array->ptr = new_ptr;
    array->ptr[array->length] = element;
    array->length++;

    return 0;
}

barn_error_or_t 
barn_delete_element_from_array(barn_array_t* array, size_t index)
{
#ifdef __BARN_SHOW_DEBUG__

    if (index < 0)
    {
        barn_debug_error("barn_delete_element_from_array: index need to be bigger than 0", __FILE_NAME__, __LINE__);
    }

#endif /* __BARN_SHOW_DEBUG__ */

    if (array == NULL || index >= array->length)
        return -1;

    for (size_t i = index; i < array->length - 1; i++)
        array->ptr[i] = array->ptr[i + 1];

    array->length--;
 
    void** new_ptr = (void**)realloc(array->ptr, array->length * sizeof(void*));
    BARN_NO_NULL(new_ptr);

    if (new_ptr != NULL || array->length == 0)
        array->ptr = new_ptr;

    return 0;
}

barn_error_or_t 
barn_delete_last_element_from_array(barn_array_t* array)
{
    if (array == NULL || array->length == 0 || array->ptr == NULL) 
        return -1;

    size_t new_length = array->length - 1;
    void** new_ptr = realloc(array->ptr, new_length * sizeof(void*));
    BARN_NO_NULL(new_ptr);
    
    if (array->size_of_element > 0) 
    {
        void* last_element = array->ptr[array->length - 1];
        free(last_element);
    }

    array->ptr = new_ptr;
    array->length = new_length;

    return 0; 
}

barn_error_or_t
barn_resize_array(barn_array_t* array, size_t n)
{
    if (array == NULL)
        return -1;

    if (n == array->length)
        return 0;

    void** new_ptr = (void**)realloc(array->ptr, n * sizeof(void*));
    BARN_NO_NULL(new_ptr);
    array->ptr = new_ptr;

    if (n < array->length) 
    {
        for (size_t i = n; i < array->length; i++) 
            if (array->size_of_element > 0) 
                free(array->ptr[i]);
    } 
    else 
    {
        for (size_t i = array->length; i < n; i++) 
            array->ptr[i] = NULL;
    }

    array->length = n;
    return 0;
}

barn_error_or_t 
barn_insert_element_in_array(barn_array_t* array, size_t index, void* element)
{
#ifdef __BARN_SHOW_DEBUG__

    if (index < 0)
    {
        barn_debug_error("barn_insert_element_in_array: index need to be bigger than 0", __FILE_NAME__, __LINE__);
    }

#endif /* __BARN_SHOW_DEBUG__ */

    if (array == NULL || element == NULL || index > array->length)
        return -1;

    void** new_ptr = (void**)realloc(array->ptr, (array->length + 1) * sizeof(void*));
    BARN_NO_NULL(new_ptr);

    array->ptr = new_ptr;

    for (size_t i = array->length; i > index; i--)
        array->ptr[i] = array->ptr[i - 1];

    array->ptr[index] = element;
    array->length++;

    return 0;
}
