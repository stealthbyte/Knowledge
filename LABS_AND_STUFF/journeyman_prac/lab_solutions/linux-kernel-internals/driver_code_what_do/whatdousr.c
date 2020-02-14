#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int fd;
	char buf[0x300] = {0};

	if (argc < 2) {
		fprintf(stderr, "Usage: %s ./node\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd, 50, 0)) {
		perror("ioctl 50");
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd, 0, 27)) {
		perror("ioctl 0");
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd, 80, 14)) {
		perror("ioctl 80");
		exit(EXIT_FAILURE);
	}

	if (lseek(fd, 3, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd, 1, 2)) {
		perror("ioctl 1");
		exit(EXIT_FAILURE);
	}

	if(read(fd, buf, sizeof(buf)) == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", buf);

	return EXIT_SUCCESS;
}

