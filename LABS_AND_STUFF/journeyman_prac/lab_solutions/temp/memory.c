#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_TOK     "/home/student/ACTP/f"

int main( int argc, char* argv[]){

    printf("Start\n");

    // printf("Page size: %d\n", getpagesize());


    key_t shm_key = ftok( SHM_TOK , 0xAF);

    printf("key: %u\n", shm_key);

    int shm_flags = 0;
    int shm_size = getpagesize();
    int shm_out = shmget(shm_key, shm_size, shm_flags);

    if ( -1 == shm_out ){

        printf("[!] Warning: No shared memory found; attempting to create\n");

        //no memory segments currently... so create it
        shm_flags |= IPC_CREAT | 0777;
        shm_out = shmget(shm_key, shm_size, shm_flags );

        if ( -1 == shm_out ){
            printf("[*] Error: Failed to create memory\n");
            perror("shmget");
            exit(-2);
        }
    }

    printf("Shared memory should have been created... \n");

    int shm_id = shm_out;

    printf("Shared Memory ID: %u\n");

    printf("Attaching to shared memory\n");

    void* shm_mem = shmat(shm_id, NULL, 0);

    if ( (void*)-1 == shm_mem ){
        printf("Error attaching to shared mem\n");
        perror("shmat");
        exit(-3);
    }

    char *shm_ch = (char*)shm_mem;

    printf("Sleeping for 20 sec to launch another\n");

    if ( 'A' != shm_ch[0] || 'F' != shm_ch[1]){
        printf("no AF found to start shared mem; writing\n");
        shm_ch[0] = 'A';
        shm_ch[1] = 'F';
        msync(shm_ch, 2, MS_SYNC);
    }
    else{
        printf("AF found! we did it!\n");
    }

    sleep(5);

    printf("Done sleeping, detaching\n");

    teardown:

    shm_out = shmdt(shm_mem);
    if ( 0 != shm_out){
        printf("Error detaching\n");
        exit(-4);
    }

    printf("Attempting to unlink shared memory\n");
    if ( 0 != shm_unlink( SHM_TOK )){
        printf("Error unlinking shared memory!\n");
        perror("shm_unlink");
        exit(-7);
    }
    printf("Shared memory unlinked\n");

    printf("Exiting...\n");

    return 0;
}