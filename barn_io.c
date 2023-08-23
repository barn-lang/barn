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

#include <barn_array.h>
#include <barn_core.h>
#include <barn_io.h>

bool 
barn_file_exists(const char* filename)
{
    FILE* f = fopen(filename, "r");

    bool f_is = f == NULL 
                    ? false
                    : true;

    if (f != NULL)
        fclose(f);

    return f_is;
}

int64_t 
barn_get_file_size(FILE* f)
{
    fseek(f, 0, SEEK_END);
    int64_t res = ftell(f);
    fseek(f, 0, SEEK_SET);

    return res;
}

char* 
barn_read_whole_file(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    BARN_NO_NULL(f);

    int64_t file_size  = barn_get_file_size(f);
    char* buffer     = calloc(sizeof(char), file_size + 1);
    BARN_NO_NULL(buffer);

    /* Return value of fread need to be equal to
     * file_size because that's how it works lol */
    assert(
        fread(buffer, 1, file_size, f) == file_size);

    buffer[file_size] = '\0';
    fclose(f);

    return buffer;
}

char*
barn_get_current_architecture() {
    #if defined(__x86_64__) || defined(_M_X64)
        return "x86_64";
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
        return "x86_32";
    #elif defined(__ARM_ARCH_2__)
        return "ARM2";
    #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
        return "ARM3";
    #elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
        return "ARM4T";
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
        return "ARM5"
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
        return "ARM6T2";
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
        return "ARM6";
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7";
    #elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7A";
    #elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7R";
    #elif defined(__ARM_ARCH_7M__)
        return "ARM7M";
    #elif defined(__ARM_ARCH_7S__)
        return "ARM7S";
    #elif defined(__aarch64__) || defined(_M_ARM64)
        return "ARM64";
    #elif defined(mips) || defined(__mips__) || defined(__mips)
        return "MIPS";
    #elif defined(__sh__)
        return "SUPERH";
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
        return "POWERPC";
    #elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
        return "POWERPC64";
    #elif defined(__sparc__) || defined(__sparc)
        return "SPARC";
    #elif defined(__m68k__)
        return "M68K";
    #else
        return "UNKNOWN"; // yo man you are using calculator?
    #endif
}