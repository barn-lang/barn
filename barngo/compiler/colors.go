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

package main

import "fmt"

// List of terminal colors
const (
	Black 		= 30		
    Red 		= 31	
    Green 		= 32		
    Yellow 		= 33		
    Blue 		= 34		
    Magenta 	= 35		
    Cyan 		= 36		
    White 		= 37	
    Gray 		= 90	
    BrightGray 	= 97
    Reset	  	= 0	
)

// This function is returning \e[(color)m
func get_color(color int) string {
	return fmt.Sprintf("\x1b[%dm", color);
}