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

#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

__BARN_FUNCTION__ char* os_current_system() {
/* __code__ ./examples/33-read-file.ba */
#if defined (__APPLE__)
/* __code__ end */
	return MacOS;
/* __code__ ./examples/33-read-file.ba */
#elif defined (__unix__)
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
struct utsname __buf = { 0 };
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
uname(&__buf);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
return strdup(__buf.sysname);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
#elif defined (_WIN32)
/* __code__ end */
	return Windows;
/* __code__ ./examples/33-read-file.ba */
#endif
/* __code__ end */
}

__BARN_FUNCTION__ bool os_file_exists(char* filename) {
	/* __code__ ./examples/33-read-file.ba */
struct stat __buf;
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
return (bool)(stat (filename, &__buf) == 0);
/* __code__ end */
}

__BARN_FUNCTION__ char* os_read_file(char* filename) {
	/* __code__ ./examples/33-read-file.ba */
FILE* f = fopen(filename, "r");
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
if (f == NULL)
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
return strdup("");
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
long size;
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
int flen = 0;
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
char* buf = "";
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
fseek(f, 0, SEEK_END);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
flen = ftell(f) + 1;
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
buf = malloc(flen * sizeof(char));
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
fseek(f, 0, SEEK_SET);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
size = fread(buf, 1, flen, f);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
if (size != flen - 1) {
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
fclose(f);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
    return buf;
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
}
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
fclose(f);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
return buf;
/* __code__ end */
}

__BARN_FUNCTION__ bool os_write_file(char* filename, char* value) {
		/* __code__ ./examples/33-read-file.ba */
FILE* f = fopen(filename, "w");
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
if (f == NULL)
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
return false;
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
int success = fprintf(f, "%s", value);
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
if (success != 0)
/* __code__ end */
/* __code__ ./examples/33-read-file.ba */
return false;
/* __code__ end */
	return true;
}

__BARN_FUNCTION__ bool string_compare(char* s1, char* s2) {
		/* __code__ ./examples/33-read-file.ba */
return __barn_string_compare(s1, s2);
/* __code__ end */
}

__BARN_FUNCTION__ int string_length(char* s) {
	/* __code__ ./examples/33-read-file.ba */
return __barn_string_length(s);
/* __code__ end */
}

__BARN_FUNCTION__ void string_reverse(char* s) {
	/* __code__ ./examples/33-read-file.ba */
return __barn_reverse(s);
/* __code__ end */
}

__BARN_FUNCTION__ void iota(int num, char* buf, int base) {
			/* __code__ ./examples/33-read-file.ba */
return __barn_iota(num, buf, base);
/* __code__ end */
}

__BARN_FUNCTION__ char* string_malloc(int size) {
	/* __code__ ./examples/33-read-file.ba */
return ((char*)malloc(size));
/* __code__ end */
}

__BARN_FUNCTION__ char* input() {
/* __code__ ./examples/33-read-file.ba */
return __barn_input();
/* __code__ end */
}

__BARN_FUNCTION__ void print(char* s) {
	/* __code__ ./examples/33-read-file.ba */
printf(s);
/* __code__ end */
}

__BARN_FUNCTION__ void printnum(int num) {
	/* __code__ ./examples/33-read-file.ba */
printf("%d", num);
/* __code__ end */
}

__BARN_FUNCTION__ void printbool(bool boo) {
		if (boo==true) {
		print("true");
	}
	else {
		print("false");
	}
}

__BARN_FUNCTION__ void free_string(char* s) {
	/* __code__ ./examples/33-read-file.ba */
free(s);
/* __code__ end */
}

__BARN_FUNCTION__ void main() {
	bool file = os_file_exists("./get_os_info.c");
	printbool(file);
	puts("");
	char* file_content = os_read_file("./get_os_info.c");
	puts(file_content);
	free_string(file_content);
	os_write_file("./get_os_info.c", "Siema");
}

