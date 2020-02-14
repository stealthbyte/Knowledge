#ifndef _HOOKING_H
#define _HOOKING_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <udis86.h>

#define JMP_PATCH_SIZE 12

#define HOOKING_SUCCESS 0
#define HOOKING_ERROR_PARAMS 1
#define HOOKING_ERROR_DISASM 2
#define HOOKING_ERROR_MALLOC 3
#define HOOKING_ERROR_MPROTECT 4

/* Entry Stub Trampoline structure */
typedef struct _entry_stub {
    uint8_t *original_entry;       //Original entry point address
    unsigned long entry_size;   //Numer of bytes compromised from original entry
    uint8_t *trampoline;           //Address of trampoline
} entry_stub_t;

/* Init function for udis86 (disassembler) library */
void udis_init();

/* Checks for jump thunk, if found, gives address that is jumped to */
uint8_t *deref_jump(uint8_t *);

/* Writes instructions for a jump to a specified target address */
void write_jump(uint8_t *jump_from, uint8_t *jump_to);

/* Overwrites code at a specified location */
void patch_code(uint8_t *target, uint8_t *patch, unsigned long len);

/* Returns the length of the instruction pointed to by addr */
unsigned int get_instruction_length(uint8_t *addr);

/**
 *  @brief Sets up the entry_stub_t structure
 *  
 *  @param stub - a pointer to the entry_stub_t struct to be set up
 *  @param original - a pointer to the function being hooked
 *  @param size - the minimum size that need be overwritten for our jmp patch
 */
int entry_stub_create(entry_stub_t *stub, uint8_t *original, unsigned long size);

/**
 *  @brief Performs the hook
 *
 *  @param stub - a pointer to the entry_stub_t struct to be set up
 *  @param hook_routine - a pointer to the routine to be called 
 */
int entry_stub_hook(entry_stub_t *stub, uint8_t *hook_routine);

/**
 *  @brief rewrites the initial bytes that were written to the trampoline 
 *         to the beginning of the function
 *
 *  @param stub - a pointer to the entry_stub_t struct
 */
int entry_stub_unhook(entry_stub_t *stub);

/**
 *  @brief clean up memory allocated during hook
 *
 *  @param stub - a pointer to the entry_stub_t struct
 */
void entry_stub_free(entry_stub_t *stub);

typedef int (*fputs_function_pointer_t)(const char *buf, FILE *stream);
#endif
