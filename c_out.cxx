#include "/home/solindek/.barn/libs/std-cxx/barn_header.hxx"



__BARN_FUNCTION__ bool string_compare(std::string s1, std::string s2) {
		/* __code__ ./examples/37-new-os-package-version.ba */
return (bool)s1.compare(s2);
/* __code__ end */
}

__BARN_FUNCTION__ int string_length(std::string s) {
	/* __code__ ./examples/37-new-os-package-version.ba */
return s.size();
/* __code__ end */
}

__BARN_FUNCTION__ void string_reverse(std::string s) {
	/* __code__ ./examples/37-new-os-package-version.ba */
return std::reverse(s.begin(), s.end());
/* __code__ end */
}

__BARN_FUNCTION__ std::string iota(int num) {
	/* __code__ ./examples/37-new-os-package-version.ba */
return std::to_string(num);
/* __code__ end */
}

__BARN_FUNCTION__ std::string input() {
/* __code__ ./examples/37-new-os-package-version.ba */
std::string str;std::cin >> str;return str;
/* __code__ end */
}

__BARN_FUNCTION__ void print(std::string s) {
	/* __code__ ./examples/37-new-os-package-version.ba */
std::cout << s;
/* __code__ end */
}

__BARN_FUNCTION__ void println(std::string s) {
	/* __code__ ./examples/37-new-os-package-version.ba */
std::cout << s << std::endl;
/* __code__ end */
}

__BARN_FUNCTION__ void printf(std::string s) {
	/* __code__ ./examples/37-new-os-package-version.ba */
std::cout << s;
/* __code__ end */
}

__BARN_FUNCTION__ void printnum(int num) {
	/* __code__ ./examples/37-new-os-package-version.ba */
std::cout << num;
/* __code__ end */
}

__BARN_FUNCTION__ void printbool(bool _bool) {
		if (_bool==true) {
		print("true");
	}
	else {
		print("false");
	}
}

__BARN_FUNCTION__ std::string os_current_system() {
/* __code__ ./examples/37-new-os-package-version.ba */
#if defined (__APPLE__)
/* __code__ end */
	return Apple;
/* __code__ ./examples/37-new-os-package-version.ba */
#elif defined (__unix__)
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
struct utsname __buf = { 0 };
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
uname(&__buf);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
return std::string(__buf.sysname);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
#elif defined (_WIN32)
/* __code__ end */
	return Windows;
/* __code__ ./examples/37-new-os-package-version.ba */
#endif
/* __code__ end */
}

__BARN_FUNCTION__ bool os_file_exists(std::string filename) {
	/* __code__ ./examples/37-new-os-package-version.ba */
#if defined (__APPLE__)
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
std::cout << "os_file_exists(string filename) is not implemented for ios" << std::endl;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
return false;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
#elif defined (__unix__)
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
struct stat __buf;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
return (bool)(stat (filename.c_str(), &__buf) == 0);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
#elif defined (_WIN32)
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
return (bool)(GetFileAttributesA(filename) == 0xFFFFFFFF);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
#else
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
std::cout << "os_file_exists(string filename) is not implemented for your system" << std::endl;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
return false;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
#endif
/* __code__ end */
}

__BARN_FUNCTION__ std::string os_read_file(std::string filename) {
	/* __code__ ./examples/37-new-os-package-version.ba */
std::string value;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
std::ifstream file(filename);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
char ac;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
if (file.is_open()) {
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
while (file) {
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
ac = file.get();
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
value.append(1, ac);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
}
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
}
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
file.close();
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
return value;
/* __code__ end */
}

__BARN_FUNCTION__ bool os_write_file(std::string filename, std::string value) {
		/* __code__ ./examples/37-new-os-package-version.ba */
std::ofstream file(filename);
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
file << value;
/* __code__ end */
/* __code__ ./examples/37-new-os-package-version.ba */
file.close();
/* __code__ end */
	return true;
}

int main() {
	std::string os = os_current_system();
	println(os);
	bool exists = os_file_exists("c_out.cxx");
	printbool(exists);
	bool file_write = os_write_file("text.txt", "Welcome everyone");
	printbool(file_write);
	std::string file_content = os_read_file("text.txt");
	println(file_content);
}

