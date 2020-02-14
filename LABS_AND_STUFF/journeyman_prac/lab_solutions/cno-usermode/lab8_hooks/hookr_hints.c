#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <elf.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE			0x1000
#define MAX_ADDR_FMT_LEN	16
#define DUMP_COLS			16

__attribute__((constructor)) void on_load(void);
__attribute__((destructor)) void on_unload(void);

int debug(char* param);
int printf_wrapper(const char* fmt, ...);
ssize_t send_wrapper(int sockfd, const void *buf, size_t len, int flags);

void* get_main_image_addr(void);
void* get_got_addr(void *image_addr);
int hook_got_entry(void* got_addr, void* orig_func_ptr, void* hook_func_ptr);
void hexdump(const unsigned char* buf, unsigned int len);
void* attempt_hooks(void* got_addr);
void create_hooking_thread(void* got_addr);


int debug(char* param) {
	// fill in
}

int printf_wrapper(const char* fmt, ...) {
	// fill in
}

ssize_t send_wrapper(int sockfd, const void *buf, size_t len, int flags) {
	// fill in
}

int hook_got_entry(void* got_addr, void* orig_func_ptr, void* hook_func_ptr) {
	// fill in
}

void* get_got_addr(void *image_addr) {
    Elf64_Ehdr* ehdr;
    Elf64_Phdr* phdr;
	Elf64_Dyn* dyn;
	int i;
	int num_dyn;
	
	ehdr = (Elf64_Ehdr*)image_addr;
	if (strncmp((char*)ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
		fprintf(stderr, "Not a valid ELF file\n");
		return NULL;
	}
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
		fprintf(stderr, "Not a 64 bit file\n");
		return NULL;
	}
	if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
		fprintf(stderr, "Not a little endian file\n");
		return NULL;
	}

	// fill in
	// hint: p_type == PT_DYNAMIC

	fprintf(stderr, "Could not find GOT\n");
	return NULL;
}

void* get_main_image_addr(void) {
	FILE* stream = NULL;
	unsigned char buf[BUF_SIZE];
	void* main_image_addr = NULL;
	char* eoi_char = NULL;

	// fill in
	// hints: 
	//		fopen("/proc/self/maps", "rb");
	//		fread
	// 		main_image_addr (void *)strtol(buf, &eoi_char, 16);

	return main_image_addr;
}

void on_load(void) {
	void* main_image_addr;
	void* got_addr;

	printf("Injected lib running...\n");
	main_image_addr = get_main_image_addr();
	if (main_image_addr == NULL) {
		return;
	}
	printf("Found main image base address at %p\n", main_image_addr);

	got_addr = get_got_addr(main_image_addr);
	if (got_addr == NULL) {
		return;
	}
	printf("Found GOT at %p\n", got_addr);

	create_hooking_thread(got_addr);
	return;
}

void on_unload(void) {
	printf("Unloading\n");
	return;
}

void create_hooking_thread(void* got_addr) {
	pthread_t tid;
	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &tattr, attempt_hooks, got_addr);
	return;
}

void* attempt_hooks(void* got_addr) {
	int hooked_send = 0;
	int hooked_printf = 0;
	while (hooked_send == 0 || hooked_printf == 0) {
		printf("Attempting to hook GOT entries...\n");
		if (hooked_printf == 0) {
			hooked_printf = hook_got_entry(got_addr, printf, printf_wrapper);
		}
		if (hooked_send == 0) {
			hooked_send = hook_got_entry(got_addr, send, send_wrapper);
		}
		sleep(1);
	}
	return NULL;
}

void hexdump(const unsigned char* buf, unsigned int len) {
	char addr_buf[MAX_ADDR_FMT_LEN + 1];
	unsigned int i;
	unsigned int j;
	unsigned int power;
	unsigned int limit;
	unsigned int pad;
	char c;
	power = 1;
	for (i = 0; i < 64; i++) {
		if (len >= power) {
			power = power << 2;
			continue;
		}
		break;
	}
	snprintf(addr_buf, MAX_ADDR_FMT_LEN, "%%0%ullx:", i);
	for (i = 0; i < len; i++) {
		if (i % DUMP_COLS == 0) {
			printf(addr_buf, i);
		}
		if (i % 2 == 0) printf(" ");
		printf("%02x", buf[i]);
		if (i % DUMP_COLS == DUMP_COLS-1 || i == (len - 1)) {
			limit = (i % DUMP_COLS);
			pad = DUMP_COLS - limit;
			if (i == (len - 1)) {
				for (j = 0; j < pad-1; j++) {
					if (j % 2 != 0) {
						printf(" ");
					}
					printf("  ");
				}
			}
			printf("  ");
			for (j = 0; j <= limit; j++) {
				c = buf[i-limit+j];
				if (isalnum(c)) {
					printf("%c", c);
				}
				else {
					printf(".");
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}
