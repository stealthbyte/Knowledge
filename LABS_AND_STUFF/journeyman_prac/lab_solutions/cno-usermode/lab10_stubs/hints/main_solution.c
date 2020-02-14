#include "hooking.h"

#define BUF_SIZE 1024

entry_stub_t stub = {0};
char global_buf[BUF_SIZE];

int fputs_wrapper(const char *buf, FILE *stream);

int main() {
    int ret = 0;
    char *one = "This is a test of fputs.\n";
    char *two = "I've been hooked.\n";
    char *three = "This should bypass the hook.\n";
    char *four = "Hook has been removed.\n";

    uint8_t *addr = NULL;
    
    //init udis86
    udis_init();
    
    //First, call function in normal state
    fputs(one, stdout);
    
    addr = (uint8_t *)fputs; //gives PLT code location
    addr = deref_jump(addr); //Gives GOT entry
    addr = (char *)(*((long *)addr)); //Gives location of shared object code

    //Setup entry_stub_t structure by calling create
    ret = entry_stub_create(&stub, addr, JMP_PATCH_SIZE);
    if (ret != HOOKING_SUCCESS) {
        printf("Failed to create stub with error %d\n", ret);
        return EXIT_FAILURE;
    }

    //Perform hook
    entry_stub_hook(&stub, (uint8_t *)fputs_wrapper);

    //Call again, should be hooked
    fputs(two, stdout);

    //Call using trampoline, should skip hook
    ((fputs_function_pointer_t)stub.trampoline)(three, stdout);

    //Unhook
    entry_stub_unhook(&stub);

    //call one more time, should not be hooked
    fputs(four, stdout);

    //cleanup
    entry_stub_free(&stub);

    return EXIT_SUCCESS;
}

int fputs_wrapper(const char *buf, FILE *stream) {
    int res;
    char *prepend = "Simon says: ";
    sprintf(global_buf, "%s", prepend);
    sprintf(global_buf + strlen(prepend), "%s", buf);
    res = ((fputs_function_pointer_t)stub.trampoline)
            (global_buf, stream);
    return res;
}
