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

import (
	"fmt"
	"time"
)

func register_time_compilation(args ArgsParser) (time.Time, bool) {
	if args.is_flag("-time") || args.is_flag("--time") || args.is_flag("-t") || args.is_flag("--t") {
		return time.Now(), true
	} else {
		return time.Now(), false
	}
}

func calculate_time_compiliation(before_time time.Time, is_time_compilation bool) {
	if is_time_compilation {
		time_now := time.Now()
		time_past := time.Unix(0, time_now.UnixNano()-before_time.UnixNano())
		nano_seconds_past := float64(time_past.Nanosecond()) / float64(1000000)
		fmt.Printf("Compilation time taken: %.3f miliseconds!\n", nano_seconds_past)
		if nano_seconds_past < 1.0 {
			fmt.Printf("Not even one milisecond!\n")
		}
	}
}
