#!/usr/bin/env python3

#
#  Barn Programming Language Organization
#  All rights reserved. Copyright (C) 2022-2023
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
# This script is made to compile all barn files in given folder
# for finding compiler errors and looking is compiler seg faulting
#

import subprocess
import sys
import os

BARN_EXTENSION = ".ba"

def find_files_with_extension(directory, extension):
    matching_files = []
    
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(extension):
                matching_files.append(os.path.join(root, file))
    
    return matching_files

def is_path_to_folder(path):
    return os.path.isdir(path)

def start_compilation_process(folder_path):
    files = find_files_with_extension(folder_path, BARN_EXTENSION)

    if len(files) == 0:
        print(f"barn: compile_all_files.py: there isn't any barn files (with extension {BARN_EXTENSION}) inside {folder_path}")
    else:
        success = 0
        for file in files:
            print(f"Compiling: {file}...")
            completed_process = subprocess.run(f"barn {file}", shell=True, text=True, capture_output=True)

            if completed_process.returncode == 0:
                print(f"Success: {completed_process.stdout}")
                success += 1
            else:
                print(f"Error message: \n{completed_process.stdout}")
        
        total_files = len(files)
        print(f"Successfully compiled {success}/{total_files}")

def main():
    if len(sys.argv) >= 2:
        folder_path = sys.argv[1]
        
        if is_path_to_folder(folder_path):
            start_compilation_process(folder_path)
        else:
            print(f"barn: compile_all_files.py: {folder_path} is not a path to folder")
    else:
        print("barn: compile_all_files.py: Expected path to folder with barn files to compile them")

main()