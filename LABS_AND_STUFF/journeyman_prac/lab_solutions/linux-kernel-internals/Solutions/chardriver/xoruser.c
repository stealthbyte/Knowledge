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

#include "xorshared.h"

#define BUF_MAX XOR_BUF_SZ

void convert_buffer(int fd, char* buffer, int len);

int main(int argc, char* argv[]) {
    int fd;
    int err;
    dev_t dev;
    int len;
    char buffer[BUF_MAX];
    unsigned char xor_val;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s string xorval\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    xor_val = atoi(argv[2]);
    dev = makedev(XOR_MAJ, XOR_MIN);
    err = mknod("mydev", S_IFCHR , dev);
    if (err == -1) {
        if (errno != EEXIST) {
            perror("mknod");
            exit(EXIT_FAILURE);
        }
    }
    
    fd = open("mydev", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, KEYCMD, xor_val) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    len = snprintf(buffer, BUF_MAX-1, "%s", argv[1]);
    if (len < 0) {
        fprintf(stderr, "failed to sprintf\n");
        exit(EXIT_FAILURE);
    }
    
    convert_buffer(fd, buffer, len);
    convert_buffer(fd, buffer, len);
    close(fd);

    return EXIT_SUCCESS;
}

void convert_buffer(int fd, char* buffer, int len) {
    if (write(fd, buffer, len) != len) {
        fprintf(stderr, "failed to write everything\n");
        exit(EXIT_FAILURE);
    }
    lseek(fd, 0, SEEK_SET);
    memset(buffer, 0, BUF_MAX);
    if (read(fd, buffer, len) != len) {
        fprintf(stderr, "failed to read everything\n");
        exit(EXIT_FAILURE);
    }
    lseek(fd, 0, SEEK_SET);
    printf("XOR yields: %s\n", buffer);
    return;
}

