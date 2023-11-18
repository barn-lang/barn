#include "/Users/bruno/.barn/std-c/barn_format.h"

#include "/Users/bruno/.barn/std-c/barn_header.h"

bool string_compare(char* s1, char* s2)
{
	return ((bool)((strcmp(s1, s2) == 0) == 1 ? 1 : 0));
}

bool __barn_string_compare(char* s1, char* s2)
{
	return ((bool)(string_compare(s1, s2)));
}

int string_length(char* s)
{
	return ((int)(strlen(s)));
}

char* int_to_string(int num, int base)
{

}

char* input()
{
	char c;
	char* str = (char*)malloc(2048);
	while((c = getchar()) != '\n') {
	 strncat(str, &c, 1);
	}
	return str;
}

void print(char* s)
{
	printf("%s", s);
}

void println(char* s)
{
	printf("%s\n", s);
}

void printnum(int num)
{
	printf("%d", num);
}

void printbool(bool to_print_bool)
{
	if (to_print_bool == true) {
		print("true");
	}
	else {
		print("false");
	}
}

void fmt_print(char* __format_start__, ... )
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

const double _MATH_EULER = 2.71828182845904523536028747135266250;

const double _MATH_LOG2 = 1.44269504088896340735992468100189214;

const double _MATH_LOG10 = 0.43429448190325182765112891891660508;

const double _MATH_PI = 3.14159265358979323846264338327950288;

const double _MATH_PI_2 = 1.57079632679489661923132169163975144;

const double _MATH_PI_4 = 0.78539816339744830961566084581987572;

long floor64(double __floor_num)
{

}

int floor32(float __floor_num)
{

}

double sqrt64(double __sqrt_num)
{

}

float sqrt32(float __sqrt_num)
{

}

double minf(double __first_min, double __second_min)
{

}

double maxf(double __first_max, double __second_max)
{

}

extern long max(long __first_max, long __second_max);

extern int okok();

void main()
{
	int i = 0;
	char* auto_str = "ok";
	float auto_flt = 4.5 * 2;
}

