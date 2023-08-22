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

#include <barn_args_parser.h>
#include <barn_time_comp.h>
#include <barn_colors.h>

barn_time_compilation_t* 
barn_start_time_compilation(barn_args_parser_t* args_parser)
{
    if (barn_is_flag(args_parser, "--time") == false || 
        barn_is_flag(args_parser, "-t") == false)
        return NULL;

    barn_time_compilation_t* time_comp = (barn_time_compilation_t*)calloc(sizeof(barn_time_compilation_t), 1);

    /* Get start time */
    gettimeofday(&time_comp->start_time, NULL);

    return time_comp;
}

double
barn_elapsed_time_convert_to_ms(barn_time_compilation_t* time_comp) 
{
    return (double)(
        (time_comp->end_time.tv_sec - time_comp->start_time.tv_sec) * 1000000 
        + time_comp->end_time.tv_usec - time_comp->start_time.tv_usec) / 1000000.0;
}

void 
barn_end_time_compilation(barn_time_compilation_t* time_comp)
{
    if (time_comp == NULL) 
        return;

    /* Get end time */
    gettimeofday(&time_comp->end_time, NULL);
}

void 
barn_print_time_compilation(barn_time_compilation_t* time_comp)
{    
    if (time_comp == NULL) 
        return;

    printf("Compilation time took: %s%fms%s\n",
        barn_get_bold_color_as_str_code(BARN_COLOR_GREEN),
        barn_elapsed_time_convert_to_ms(time_comp),
        barn_get_bold_color_as_str_code(BARN_COLOR_RESET));
}