#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>

#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/audit.h>

#define ArchField offsetof(struct seccomp_data, arch)

#define Allow(syscall)\
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_##syscall, 0, 1),\
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

struct sock_filter filter[] = {
	/* validate arch */
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, ArchField),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, AUDIT_ARCH_X86_64, 1, 0),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),

	/* load syscall */
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

	/* list of allowed syscalls */
	Allow(exit_group),	/* exits a process */
	Allow(brk), 		/* for malloc(), inside of libc - printf may call */
	Allow(mmap), 		/* also for malloc(), printf may call */
	Allow(munmap),		/* for free(), inside libc - printf may call */
	Allow(write), 		/* called by printf */
	Allow(fstat), 		/* called by printf */
	Allow(openat), 		/* called by fopen */
	Allow(lseek), 		/* called by fseek */
	Allow(read), 		/* called by fread */
	Allow(close), 		/* called by fclose */

	/* and if we don't match any above syscall, die */
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),
//	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_TRAP),	/* Use this with strace to find which syscall killed the process */
};

struct sock_fprog filterprog = {
	.len = sizeof(filter)/sizeof(filter[0]),
	.filter = filter
};

void enable_seccomp()
{
	/* set up the restricted environment */
	if(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("Could not start seccomp:");
		exit(1);
	}
	if(prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &filterprog) == -1) {
		perror("Could not start seccomp:");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	enable_seccomp();

	/* printf only writes to stdout, but for some reason it stats it.  */
	printf("Hello World!\n");


	/* but if you know the secret password, you can
	 * compromise the program, and we spawn a shell
	 */
	if(argc > 1 && strcmp(argv[1], "haxor") == 0)
	{
		FILE *f = fopen("/etc/passwd", "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);	/* same as rewind(f) */

		char *string = (char*)calloc(fsize + 1, sizeof(char));
		fread(string, fsize, 1, f);
		fclose(f);

		printf("%s\n", string);
	}


	return 0;
}
