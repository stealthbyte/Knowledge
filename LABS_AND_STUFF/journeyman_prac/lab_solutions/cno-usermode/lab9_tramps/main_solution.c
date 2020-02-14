#include "hooking.h"

#define BUF_SIZE 1024

void func1(unsigned long arg);
int func2(unsigned long arg);

int main() {
	uint8_t *func1_addr = NULL;
	uint8_t ret_patch = 0xc3;

	printf("Calling func1 for the first time\n");
	func1(42);

	//Simple return patch
	func1_addr = deref_near_jump((uint8_t *)func1);
	patch_code(func1_addr, &ret_patch, sizeof(ret_patch));

	printf("Calling return patched version of func1\n");
	func1(42);

	//jump patch
	write_absolute_jump(deref_near_jump((uint8_t *)func1), deref_near_jump((uint8_t *)func2));

	printf("Calling jump patched version of func1\n");
	func1(42);

	return EXIT_SUCCESS;
}

void func1(unsigned long arg) {
	printf("func1 was called with %u\n", arg);
}

int func2(unsigned long arg) {
	printf("func2 was called with %u\n", arg);
	return 0x42;
}
