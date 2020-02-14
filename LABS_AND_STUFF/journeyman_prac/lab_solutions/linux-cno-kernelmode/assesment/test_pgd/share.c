#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd;
	void* addr;
	fd = shm_open("myshem", O_RDWR | O_CREAT, 0777);
	if (fd == -1) {
		printf("Bad open\n");
		return -1;
	}

	addr = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == (void*)-1) {
		printf("Bad map");
		return -1;
	}

	while (1) {
		sleep(1);
	}

	return 0;
}
