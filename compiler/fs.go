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
	"io/ioutil"
	"os"
	"runtime"
)

func get_file_value(filename string) string {
	content, _ := ioutil.ReadFile(filename)
	return string(content)
}

func get_barn_libs_directory() string {
	if runtime.GOOS == "linux" || runtime.GOOS == "darwin" {
		return os.Getenv("HOME") + "/.barn/libs/"
	} else {
		return "/.barn/libs/"
	}
}
