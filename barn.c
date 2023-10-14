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

__BARN_FUNCTION__ char* iota(int num, int base)
{
	return iota(num, base);
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

__BARN_FUNCTION__ void main()
{
	println("IT FUCKING WORKS LOLOLOL FUCK CORRUPTED BUFFERS IN C");
}

