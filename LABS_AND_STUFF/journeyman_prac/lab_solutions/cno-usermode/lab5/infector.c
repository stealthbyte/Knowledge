#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>


// helper function to open and map a file into your address space
static int openAndMap(char* path, size_t* out_size, void** out_data);

int main(int argc, char* argv[]) {
	int err = 0;
	int targ_fd = -1;
	size_t targ_sz = 0;
	void* targ_data = NULL;
	int injcode_fd = -1;
	size_t injcode_sz = 0;
	void* injcode_data = NULL;

	if (argc < 3) {
		printf("Usage: %s target_elf pic_shellcode\n", argv[0]);
		err = -1;	
		goto END;
	}

	// open and map the target and input
	targ_fd = openAndMap(argv[1], &targ_sz, &targ_data);
	if (targ_fd < 0) {
		goto END;
	}

	// open and map the injection pic code
	injcode_fd = openAndMap(argv[2], &injcode_sz, &injcode_data);

	printf("Target file size = 0x%x\n", targ_sz);
		
	// injcode_data is the payload.bin mapped into the address space
	// injcode_sz is the size of payload.bin
	// targ_data is the ssh elf64 file mapped into the address space
	// targ_sz is the size of the mapped ssh file
	
	// find gaps after executable pages in the target file
	// fill the gap with the injcode
	// update the Elf64.e_entry to point to the injcode
	// fill out the 0x4141414141414141 in the injcode with the old e_entry
	
	
	/*
 	 *  Students fill in code here
 	 */
	

END:
	if (targ_fd >= 0) {
		close(targ_fd);
	}
	if (targ_data) {
		munmap(targ_data, targ_sz);
	}
	if (injcode_fd >= 0) {
		close(injcode_fd);
	}
	if (injcode_data) {
		munmap(injcode_data, injcode_sz);
	}

	return err;
}

// returns the fd
int openAndMap(char* path, size_t* out_size, void** out_data) {
	int fd = -1;
	off_t endoff = -1;
	void* data = NULL;
	
	*out_size = 0;
	*out_data = NULL;

	fd = open(path, O_APPEND | O_RDWR);
	if (fd < 0) {
		printf("Error: unable to open file \"%s\"\n", path);
		perror(NULL);
		goto ERR_END;
	}

	// get size
	endoff = lseek(fd, 0, SEEK_END);
	if (endoff < 0) {
		printf("Error: unable to seek in file \"%s\"\n", path);
		perror(NULL);
		goto ERR_END;
	}
	
	*out_size = (size_t)endoff;

	endoff = lseek(fd, 0, SEEK_SET);
	if (endoff < 0) {
		printf("Error: unable to seek back in file \"%s\"\n", path);
		perror(NULL);
		goto ERR_END;
	}

	// map
	data = mmap(
		0,
		*out_size,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd,
		0
	);

	if (data == MAP_FAILED) {
		printf("Error: unable to map file \"%s\"\n", path);
		perror(NULL);
		goto ERR_END;
	}

	*out_data = data;
	
	goto END;
ERR_END:
	if (fd != -1) {
		close(fd);
	}
	fd = -1;
END:
	return fd;
}

