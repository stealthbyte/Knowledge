#include <grp.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>

#define UID_BASE	1337
#define UID_MAX		1341
#define NUM_FLAGS	6

int drop_privs(uid_t new_uid, gid_t new_gid);
pid_t create_child(int i, char* buffer);
void check_time(int i);

char flags[][80] = {
		"TUX_WOULD_KILL_HODOR",
		"TUX_WOULD_KILL_TYRION",
		{'T', 'U', 'X', '_', 'W', 'O', 'U', 'L', 'D', '_', 'K', 'I', 'L', 'L', '_', 'J', 'O', 'F', 'F', 'R', 'E', 'Y', '\0'},
		//{ 0x33, 0x93, 0x31, 0x2c, 0x06, 0xb0, 0x1f, 0xa0, 0x6d, 0x92, 0xf1, 0xe2, 0xbe, 0xb7, 0xbc, 0x0c, 0x33, 0x84, 0x12, 0xaa, 0x5e, 0xb1},
		"TUX_WOULD_KILL_SAMWELL",
		"TUX_WOULD_KILL_PETYR",
		"TUX_WOULD_KILL_RAMSAY",
};
char flags2[][80] = {
		"TUX_WOULDNT_KILL_HODOR",
		"TUX_WOULDNT_KILL_TYRION",
		"TUX_WOULDNT_KILL_JOFFREY",
		"TUX_WOULDNT_KILL_SAMWELL",
		"TUX_WOULDNT_KILL_PETYR",
		"TUX_WOULDNT_KILL_RAMSAY",
};

int main() {
	pid_t pids[NUM_FLAGS];
	void* fixed_buf;

	srand(0);
	int i;

	if (geteuid() != 0) {
		printf("Run this with sudo!\n");
		exit(EXIT_FAILURE);
	}

	fixed_buf = mmap((void*)0x4DEADB000, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (fixed_buf == (void*)-1) {
		perror("mmap");
		printf("Please see the instructor about this issue\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_FLAGS; i++) {
		pids[i] = create_child(i, fixed_buf);
	}

	getc(stdin);
	for (i = 0; i < NUM_FLAGS; i++) {
		kill(pids[i], SIGCONT);
	}
	return EXIT_SUCCESS;
}

int drop_privs(uid_t new_uid, gid_t new_gid) {
	uid_t current_uid;
	gid_t current_gid;
	
	current_uid = geteuid();
	current_gid = getegid();

	if (current_uid == 0) {
		if (setgroups(1, &new_gid) == -1) {
			perror("setgroups");
			exit(EXIT_FAILURE);
		}
	}
	if (new_gid != current_gid) {
		if (setregid(new_gid, new_gid) == -1) {
			perror("setregid");
			exit(EXIT_FAILURE);
		}
	}

	if (new_uid != current_uid) {
		if (setreuid(new_uid, new_uid) == -1) {
			perror("setreuid");
			exit(EXIT_FAILURE);
		}
	}

	/* verify success */
	if (new_gid != current_gid) {
		if (setegid(current_gid) != -1 || getegid() != new_gid) {
			fprintf(stderr, "Change of GID failed\n");
			exit(EXIT_FAILURE);
		}
	}

	if (new_uid != current_uid) {
		if (seteuid(current_uid) != -1 || geteuid() != new_uid) {
			fprintf(stderr, "Change of UID failed\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

pid_t create_child(int i, char* buffer) {
	pid_t pid;
	int j;
	char xorchr;
	pid = fork();
	if (pid == -1) {
		perror("fork");
		printf("Please see the instructor about this issue\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		for (j = 0; j < 80; j++) {
			//xorchr = rand();
			//flags[i][j] = flags[i][j] ^ xorchr;
		}
		drop_privs(UID_BASE+i, UID_MAX-i);
		strcpy(buffer, flags[i]);
		//printf("UID is %d, GID is %d, fixed_buf is %s\n", getuid(), getgid(), buffer);
		raise(SIGSTOP);
		check_time(i);
		exit(EXIT_SUCCESS);
	}
	return pid;
}

void check_time(int i) {
	FILE* f;
	unsigned long long startime;
	char buffer[4096];
	f = fopen("/proc/self/stat", "r");
	if (f == NULL) {
		perror("fopen");
		printf("Please see the instructor about this issue\n");
		exit(EXIT_FAILURE);
	}
	fgets(buffer, 4096, f);
	sscanf(buffer, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %*ld %*ld %llu", &startime);
	//printf("Startime is %llu\n", startime);
	if (startime == 0) {
		printf("[%s]\n", flags[i]);
	}
	fclose(f);
	return;
}
