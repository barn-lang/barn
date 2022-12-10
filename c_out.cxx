#include "/home/solindek/.barn/libs/std-cxx/barn_header.hxx"

// int __barn_string_length(char* str) {
//     int i = 0;
//     while (str[i] != '\0') ++i;
//     return i;
// }

// bool __barn_string_compare(char* str1, char* str2) {
//     int length_s1 = __barn_string_length(str1);
//     int length_s2 = __barn_string_length(str2);

//     if (length_s1 == length_s2) {
//         for (int i = 0; i < length_s1; i++) {
//             if (str1[i] != str2[i]) {
//                 return false;
//             }
//         }
//     } else {
//         return false;
//     }

//     return true;
// }

// void __barn_reverse(char* s) {
//     int c, i, j;
//     for (i = 0, j = __barn_string_length(s)-1; i < j; i++, j--) {
//         c = s[i];
//         s[i] = s[j];
//         s[j] = c;
//     }
// }

// void __barn_iota(int _num, char* _str, int _base) {
//     int i = 0;
//     bool is_neg = false;
    
//     if (_num == 0) { 
//         _str[i++] = '0';
//         _str[i] = '\0';
//         return;
//     } 
//     if (_num < 0 && _base == 10) {
//         is_neg = true;
//         _num = -_num;
//     }
//     while (_num != 0) {
//         int rem = _num % _base;
//         _str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
//         _num = _num / _base;
//     }
//     if (is_neg)
//         _str[i++] = '-';
//     _str[i] = '\0';
//     __barn_reverse(_str);
// }

// char* __barn_input() {
//     char c;
//     char* str = (char*)malloc(2048);
// 	while((c = getchar()) != '\n') {
//         strncat(str, &c, 1);
//     }
//     return str;
// }


__BARN_FUNCTION__ bool string_compare(std::string s1, std::string s2) {
		/* __code__ ./examples/36-generating-with-cxx.ba */
return (bool)s1.compare(s2);
/* __code__ end */
}

__BARN_FUNCTION__ int string_length(std::string s) {
	/* __code__ ./examples/36-generating-with-cxx.ba */
return s.size();
/* __code__ end */
}

__BARN_FUNCTION__ void string_reverse(std::string s) {
	/* __code__ ./examples/36-generating-with-cxx.ba */
return std::reverse(s.begin(), s.end());
/* __code__ end */
}

__BARN_FUNCTION__ std::string iota(int num) {
	/* __code__ ./examples/36-generating-with-cxx.ba */
return std::to_string(num);
/* __code__ end */
}

__BARN_FUNCTION__ std::string input() {
/* __code__ ./examples/36-generating-with-cxx.ba */
std::string str;std::cin >> str;return str;
/* __code__ end */
}

__BARN_FUNCTION__ void print(std::string s) {
	/* __code__ ./examples/36-generating-with-cxx.ba */
std::cout << s;
/* __code__ end */
}

__BARN_FUNCTION__ void printf(std::string s) {
	/* __code__ ./examples/36-generating-with-cxx.ba */
std::cout << s;
/* __code__ end */
}

__BARN_FUNCTION__ void printnum(int num) {
	/* __code__ ./examples/36-generating-with-cxx.ba */
std::cout << num;
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

__BARN_FUNCTION__ void main() {
	std::string example_string = "Hello World";
	print(example_string);
}

