#include "hooking.h"

#define SIZE_OF_JMP 6

uint8_t *deref_jump(uint8_t *target) {
    printf("Not supported\n");
    return NULL;
}

void write_jump(uint8_t *jump_from, uint8_t *jump_to) {
    printf("Not supported\n");
}

void patch_code(uint8_t *target, uint8_t *patch, unsigned long len) {
    printf("Not supported\n");
}
