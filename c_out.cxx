#include "/home/solindek/.barn/libs/std-cxx/barn_header.hxx"



__BARN_FUNCTION__ bool string_compare(std::string s1, std::string s2) {
		/* __code__ ./tests/test-04-auto-variables.ba */
return (bool)s1.compare(s2);
/* __code__ end */
}

__BARN_FUNCTION__ int string_length(std::string s) {
	/* __code__ ./tests/test-04-auto-variables.ba */
return s.size();
/* __code__ end */
}

__BARN_FUNCTION__ void string_reverse(std::string s) {
	/* __code__ ./tests/test-04-auto-variables.ba */
return std::reverse(s.begin(), s.end());
/* __code__ end */
}

__BARN_FUNCTION__ std::string iota(int num) {
	/* __code__ ./tests/test-04-auto-variables.ba */
return std::to_string(num);
/* __code__ end */
}

__BARN_FUNCTION__ std::string input() {
/* __code__ ./tests/test-04-auto-variables.ba */
std::string str;std::cin >> str;return str;
/* __code__ end */
}

__BARN_FUNCTION__ void print(std::string s) {
	/* __code__ ./tests/test-04-auto-variables.ba */
std::cout << s;
/* __code__ end */
}

__BARN_FUNCTION__ void println(std::string s) {
	/* __code__ ./tests/test-04-auto-variables.ba */
std::cout << s << std::endl;
/* __code__ end */
}

__BARN_FUNCTION__ void printf(std::string s) {
	/* __code__ ./tests/test-04-auto-variables.ba */
std::cout << s;
/* __code__ end */
}

__BARN_FUNCTION__ void printnum(int num) {
	/* __code__ ./tests/test-04-auto-variables.ba */
std::cout << num;
/* __code__ end */
}

__BARN_FUNCTION__ void printbool(bool _bool) {
		if ((_bool==true)) {
		print("true");
	}
	else {
		print("false");
	}
}

__BARN_FUNCTION__ int call_example_function() {
	return 100;
}

int main() {
	float i = 1;
	std::string i1 = "string";
	int i2 = call_example_function();
	bool i3 = false;
}

