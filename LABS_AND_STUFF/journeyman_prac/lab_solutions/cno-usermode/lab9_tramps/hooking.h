#ifndef _HOOKING_H
#define _HOOKING_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>

/* Checks for jump thunk, if found, gives address that is jumped to */
void* deref_near_jump(uint8_t *target);
void* deref_plt_jump(uint8_t *target);

/* Writes instructions for a jump to a specified target address */
void write_absolute_jump(uint8_t *jump_from, uint8_t *jump_to);

/* Overwrites code at a specified location */
void patch_code(void* target, void* patch, unsigned long len);

#endif
