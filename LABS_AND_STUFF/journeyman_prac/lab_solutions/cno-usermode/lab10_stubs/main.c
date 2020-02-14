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
    
    //TODO: Setup entry_stub_t structure by calling create

    //TODO: Perform hook

    //Call again, should be hooked
    fputs(two, stdout);

    //TODO: Call using trampoline, should skip hook

    //TODO: unhook

    //call one more time, should not be hooked
    fputs(four, stdout);

    //TODO: cleanup

    return EXIT_SUCCESS;
}

int fputs_wrapper(const char *buf, FILE *stream) {
    int res;
    char *prepend = "Simon says: ";
    sprintf(global_buf, "%s", prepend);
    sprintf(global_buf + strlen(prepend), "%s", buf);
    //TODO: Call original function using trampoline
    return res;
}
