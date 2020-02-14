#include "hooking.h"

#define MAX_INSTRUCTION_SIZE 15
#define SIZE_OF_JMP 6

ud_t g_ud_obj;

void udis_init() {
    ud_init(&g_ud_obj);
    ud_set_mode(&g_ud_obj, 64);
    ud_set_syntax(&g_ud_obj, UD_SYN_ATT);
}

unsigned int get_instruction_length(uint8_t *addr) {
    int len = 0;
    ud_set_input_buffer(&g_ud_obj, addr, MAX_INSTRUCTION_SIZE); 
    len = ud_disassemble(&g_ud_obj);
    //printf("%d:\t%s\n", len, ud_insn_asm(&g_ud_obj));
    return len;
}

uint8_t *deref_jump(uint8_t *target) {
    unsigned char jmp_bytes[] = {0xFF, 0x25};
    if (!target) {
        return NULL;
    }
    if (!memcmp(target, jmp_bytes, 2)) {
        int offset =  *((int *)(target + 2)); 
        return target + offset + SIZE_OF_JMP; 
    }
    else {
        return target;
    }
}

void write_jump(uint8_t *jump_from, uint8_t *jump_to) {
    /*  currently:
     *      movabs rax, 0x0000000000000000
     *      jmp rax */
    unsigned char jmp_template[] = {0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0};

    *((long *)(jmp_template + 2)) = (long)(jump_to);
    patch_code(jump_from, jmp_template, JMP_PATCH_SIZE);
}

void patch_code(uint8_t *target, uint8_t *patch, unsigned long len) {
    if (!target || !patch || !len) {
        printf("Invalid parameters\n");
        return;
    }
    uint8_t *target_page = (uint8_t *)((long)target & 0xfffffffffffff000);
    if (mprotect(target_page, len + (target - target_page), 
                PROT_READ | PROT_WRITE | PROT_EXEC)) {
        printf("Failed to change permissions of memory in patch:\n%s\n", strerror(errno));
    }
    memcpy(target, patch, len);
}

int entry_stub_create(entry_stub_t *stub, uint8_t *original, unsigned long size){
    printf("Unsupported\n");
    return 0;
}

int entry_stub_hook(entry_stub_t *stub, uint8_t *hook_routine) {
    printf("Unsupported\n");
    return 0;
}

int entry_stub_unhook(entry_stub_t *stub) {
    printf("Unsupported\n");
    return 0;
}

void entry_stub_free(entry_stub_t *stub) {
    printf("Unsupported\n");
}
