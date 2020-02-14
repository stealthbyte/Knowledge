#include <fcntl.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int task1_smid;
	int task2_smid;
	void* task1_smp;
	void* task2_smp;
	int pagesize = getpagesize();
	key_t smkey;

	smkey = ftok("/home/student/Documents/linux-internals/labs/lab08/shm_file.maybe", 1);	
	printf("smkey: %d\n", smkey);

	task1_smid = shmget(smkey, pagesize, 0777);
	if (task1_smid == -1){
		printf("ERROR: shmget\n");
		goto cleanup;
	}
	printf("task1_smid: %d\n", task1_smid);

	task1_smp = shmat(task1_smid, NULL, 0);
	if (task1_smp == (void*)-1){
		printf("ERROR: shmat\n");
		return -1;
	}
	printf("task1_smp: %p\n", task1_smp);

	task2_smid = shmget(smkey, pagesize, SHM_REMAP);
	if (task2_smid == -1){
		printf("ERROR: shmget\n");
		goto cleanup;
	}
	printf("task2_smid: %d\n", task2_smid);

	task2_smp = shmat(task2_smid, task1_smp, SHM_RND);
	if (task2_smp == (void*)-1){
		printf("ERROR: shmat\n");
		return -1;
	}
	printf("task2_smp: %p\n", task2_smp);

	cleanup:
	if(shm_unlink("/home/student/Documents/linux-internals/labs/lab08/shm_file.maybe") == -1){ printf("ERROR: unlink\n"); }
	if(task1_smp != (void*)-1) { shmdt(task1_smp); }
	if(task2_smp != (void*)-1) { shmdt(task2_smp); }
	return 0;
}
