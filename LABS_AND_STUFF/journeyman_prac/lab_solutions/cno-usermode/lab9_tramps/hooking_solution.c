#include "hooking.h"

void* deref_plt_jump(uint8_t *target) {
	uint64_t* jmp_addr;
	unsigned int offset;
	unsigned char opcode[] = {0xFF, 0x25}; /* rip relative jmp */
	unsigned int opcode_len = sizeof(opcode);
	unsigned int inst_len = 6; /* see size for rip relative jmp */
	if (memcmp(target, opcode, opcode_len) == 0) {
		offset = *((uint64_t *)(target + opcode_len));
		jmp_addr = (uint64_t*)(target + offset + inst_len);
		return (void*)(*jmp_addr);
	}
	return target;
}

void* deref_near_jump(uint8_t *target) {
	unsigned int offset;
	unsigned char opcode[] = {0xE9}; /* near jmp */
	unsigned int opcode_len = sizeof(opcode);
	unsigned int inst_len = 5; /* see size for near jmp */
	if (memcmp(target, opcode, opcode_len) == 0) {
		offset = *((unsigned int *)(target + opcode_len));
		return target + offset + inst_len;
	}
	return target;
}

void patch_code(void* target, void* patch, unsigned long len) {
	void* target_page = (void*)((size_t)target & 0xfffffffffffff000);
	unsigned long protlen = len + (target - target_page);
	if (mprotect(target_page, protlen, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
		fprintf(stderr, "Failed to change permissions of memory before patch:\n%s\n", strerror(errno));
	}
	memcpy(target, patch, len);
	if (mprotect(target_page, protlen, PROT_READ | PROT_EXEC) == -1) {
		fprintf(stderr, "Failed to change permissions of memory back to original:\n%s\n", strerror(errno));
	}
	return;
}

void write_absolute_jump(uint8_t *jump_from, uint8_t *jump_to) {
	/*	currently:
	 *		movabs rax, 0x0000000000000000
	 *		jmp rax */
	unsigned char jmp_template[] = {0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0};
	*((uint64_t*)(jmp_template + 2)) = (uint64_t)(jump_to);
	patch_code(jump_from, jmp_template, 12);
	return;
}

