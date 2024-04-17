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

#ifndef __BARN_VARIABLE_MODIFICATION__
#define __BARN_VARIABLE_MODIFICATION__

#include <barn_core.h>

#include <barn_variables.h>
#include <barn_parser.h>
#include <barn_types.h>
#include <barn_nodes.h>

void barn_parser_variable_asn     (barn_parser_t* parser, barn_parser_access_element_t* element); // =
void barn_parser_variable_plusasn (barn_parser_t* parser, barn_parser_access_element_t* element); // +=
void barn_parser_variable_minusasn(barn_parser_t* parser, barn_parser_access_element_t* element); // -= 
void barn_parser_variable_mulasn  (barn_parser_t* parser, barn_parser_access_element_t* element); // *=
void barn_parser_variable_divasn  (barn_parser_t* parser, barn_parser_access_element_t* element); // /=
void barn_parser_variable_modasn  (barn_parser_t* parser, barn_parser_access_element_t* element); // /=

void barn_parser_variable_incrementation(barn_parser_t* parser, barn_parser_access_element_t* element); // ++
void barn_parser_variable_decrementation(barn_parser_t* parser, barn_parser_access_element_t* element); // --

#endif /* __BARN_VARIABLE_MODIFICATION__ */