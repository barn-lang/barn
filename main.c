#include <stdio.h>

int main(int argc, char** argv) {
	while (*argv != NULL) {
		println("%s\n", *argv);
		*argv++;
	}
}
