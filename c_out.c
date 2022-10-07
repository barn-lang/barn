#include "lib/std-c/barn_header.h"

int __barn_string_length(char* str) {
    int i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

bool __barn_string_compare(char* str1, char* str2) {
    int length_s1 = __barn_string_length(str1);
    int length_s2 = __barn_string_length(str2);

    if (length_s1 == length_s2) {
        for (int i = 0; i < length_s1; i++) {
            if (str1[i] != str2[i]) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;
}

void __barn_reverse(char* s) {
    int c, i, j;
    for (i = 0, j = __barn_string_length(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void __barn_iota(int _num, char* _str, int _base) {
    int i = 0;
    bool is_neg = false;
    
    if (_num == 0) { 
        _str[i++] = '0';
        _str[i] = '\0';
        return;
    } 
    if (_num < 0 && _base == 10) {
        is_neg = true;
        _num = -_num;
    }
    while (_num != 0) {
        int rem = _num % _base;
        _str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        _num = _num / _base;
    }
    if (is_neg)
        _str[i++] = '-';
    _str[i] = '\0';
    __barn_reverse(_str);
}

char* __barn_input() {
    char c;
    char* str = (char*)malloc(2048);
	while((c = getchar()) != '\n') {
        strncat(str, &c, 1);
    }
    return str;
}


__BARN_FUNCTION__ void main() {
	float pi = 3.15;
	float circle = (pi*(20*20));
}

