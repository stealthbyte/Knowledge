#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int main() {
	uint64_t csum = 0;
	uint64_t word;
	ssize_t bytes_read;
	while (1) {
		word = 0;
		bytes_read = read(STDIN_FILENO, (void*)&word, 8);
		//fprintf(stderr, "PRE: csum is %#016llx\n", csum);
		csum += word;
		if (bytes_read == 0) {
			break;
		}
	}
	//fprintf(stderr, "csum is %#016llx\n", csum);
	write(STDOUT_FILENO, (void*)&csum, 8);
	return 0;
}
