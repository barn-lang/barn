#include "/Users/bruno/.barn/std-c/barn_format.h"

#include "/Users/bruno/.barn/std-c/barn_header.h"

__BARN_FUNCTION__ bool string_compare(char* s1, char* s2)
{
	return ((bool)((strcmp(s1, s2) == 0) == 1 ? 1 : 0));
}

__BARN_FUNCTION__ bool __barn_string_compare(char* s1, char* s2)
{
	return ((bool)(string_compare(s1, s2)));
}

__BARN_FUNCTION__ int string_length(char* s)
{
	return ((int)(strlen(s)));
}

__BARN_FUNCTION__ char* int_to_string(int num, int base)
{

}

__BARN_FUNCTION__ char* input()
{
	char c;
	char* str = (char*)malloc(2048);
	while((c = getchar()) != '\n') {
	 strncat(str, &c, 1);
	}
	return str;
}

__BARN_FUNCTION__ void print(char* s)
{
	printf("%s", s);
}

__BARN_FUNCTION__ void println(char* s)
{
	printf("%s\n", s);
}

__BARN_FUNCTION__ void printnum(int num)
{
	printf("%d", num);
}

__BARN_FUNCTION__ void printbool(bool to_print_bool)
{
	if (to_print_bool == true) { printf("true"); } else { printf("false"); }
}

__BARN_FUNCTION__ void fmt_print(char* __format_start__, ... )
{
	__barn_start_format();
	int strlength = string_length(__format_start__);
	int i = 0;
	while (i < strlength) {
		char curr_char = ' ';
		curr_char = __format_start__[i];
		if (curr_char == '%') {
			i ++;
			curr_char = __format_start__[i];
			if (curr_char == 's') {
				char* __str = __barn_format_get_value_string();
				print(__str);
			}
			else if (curr_char == 'd') {
				int __decimal = __barn_format_get_value_int();
				printf("%d", __decimal);
			}
			else if (curr_char == 'x') {
				int __hexadecimal = __barn_format_get_value_int();
				printf("%x", __hexadecimal);
			}
			else if (curr_char == 'o') {
				int __octal = __barn_format_get_value_int();
				printf("%o", __octal);
			}
			else if (curr_char == 'f') {
				double __double = __barn_format_get_value_double();
				printf("%lf", __double);
			}
			else if (curr_char == 'l') {
				long __long = __barn_format_get_value_long();
				printf("%ld", __long);
			}
		}
		else {
			printf("%c", curr_char);
		}
		i ++;
	}
}

__BARN_FUNCTION__ void main()
{
	int i = 0;
	char* auto_str = "ok";
	float auto_flt = 4.5 * 2;
}

