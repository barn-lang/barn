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

#include <barn_string.h>

// TODO: rewrite this shit by using: https://wiki.osdev.org/Random_Number_Generator

static uint64_t random_number_seed = 0;

uint64_t
barn_get_random_seed()
{
    /* Get stack pointer */
    void* p = NULL;
    uint64_t sp = ((uint64_t)(&p));

    struct timeval tm;
    gettimeofday(&tm, NULL);

    uint64_t crazy_num = (tm.tv_sec * tan(tm.tv_usec));

    if (crazy_num == 0)
        return sp + tm.tv_usec + tm.tv_sec * M_2_SQRTPI;

    return crazy_num;
}

double
barn_random_number()
{
    double random_num = (random_number_seed = barn_get_random_seed()) / 10000000000.00000000;

    if (random_num > 1)
        random_num -= (int)random_num;

    return random_num;
}

double
barn_random_number_in_range(double min, double max)
{
    return floor(barn_random_number() * max) + min;
}

char*
barn_generate_random_ascii_bytes(size_t n)
{
    char* buf = barn_create_allocated_string();

    for (int i = 0; i < n; i++)
        if (n % 2 == 0)
            barn_append_char_to_allocated_string(buf, barn_random_number_in_range(65, 90));
        else
            barn_append_char_to_allocated_string(buf, barn_random_number_in_range(97, 122));
}
