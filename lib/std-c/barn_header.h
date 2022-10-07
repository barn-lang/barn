#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __BARN_FUNCTION__
#define __BARN_GLOBAL_VARIABLE__

typedef int bool;

#define true  0x1
#define false 0x0

int __barn_string_length(char* str);
bool __barn_string_compare(char* str1, char* str2);

// OpenGL binding stuff
__BARN_FUNCTION__ void display_func();