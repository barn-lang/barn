#!/usr/bin/env python3

#
#  Barn Programming Language Organization
#  All rights reserved. Copyright (C) 2022-2024
#   
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#   
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#   
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

#
# This script will generate a big barn file with given amount of lines
# in every line there will be some tokens for barn lexer to tokenize 
# so we can check it performance
#

import subprocess
import sys
import os

BARN_EXTENSION = ".ba"
BARN_LINE      = "identifier 123 \"string\" 123.10 // comment"
BARN_FILE      = "./generated.ba"

def generate_lines(line_amount):
    file_path = BARN_FILE
    with open(file_path, "a") as file:
        for i in range(0, line_amount):
            file.write(f"{BARN_LINE}\n")

    print(f"Successfully wrote {line_amount} lines to file named: \"{file_path}\"")

def main():
    if len(sys.argv) >= 2:
        try:
            line_amount = int(sys.argv[1])
            generate_lines(line_amount)
        except ValueError:
            print("barn: compile_all_files.py: Given amount of lines is not correct, Can't convert given value to int")
    else:
        print("barn: compile_all_files.py: Expected amount of lines to generate: ./barn_lines_generator.py <amount>")

main()