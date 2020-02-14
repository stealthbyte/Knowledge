#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFKEY		"ACTP"
#define BADKEYCHAR	".~ &$"

int main(int argc, char* argv[]) {
	char* key = DEFKEY;
	char* pt = NULL;
	char c;
	size_t i = 0;
	if (argc < 2) {
		printf("Argument required\n");
		exit(-1);
	}

	pt = argv[1];

	if (argc > 2) {
		key = argv[2];
		for (i=0; i<strlen(BADKEYCHAR); i++) {
			if (strchr(key, BADKEYCHAR[i])) {
				// found bad char
				fprintf(stderr, "Unable to use key with character '%c'\n", BADKEYCHAR[i]);
				exit(-1);
			}
		}
	}

	for (i=0; i<strlen(pt); i++) {
		c = pt[i] ^ key[i%strlen(key)];
		write(STDOUT_FILENO, &c, 1);
		if ((i%8) == 7) {
			fwrite("  ", 1, 2, stdout);	
		}
	}

	putc('\n', stdout);
	
	_exit(0);
	return 0;
}
