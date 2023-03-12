#pragma once

#include <iostream>
#include <fstream>

#include <string>
#include <compare>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>

#if defined(__unix__)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <sys/utsname.h>
#elif defined(_WIN32)
    #include <windows.h>
#endif

#define __BARN_FUNCTION__
#define __BARN_GLOBAL_VARIABLE__

#define __use__(x) {x;}

// int __barn_string_length(char* str);
// bool __barn_string_compare(char* str1, char* str2);

// OpenGL binding stuff
// __BARN_FUNCTION__ void display_func();