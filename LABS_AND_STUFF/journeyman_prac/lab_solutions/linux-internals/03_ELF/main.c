#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void print_usage(char* name);
int parse_elf(unsigned char* data, unsigned int datalen);
void print_program_header_type(Elf64_Word type);
void print_section_header_type(Elf64_Word type);
void print_object_file_type(Elf64_Half type);
void print_program_header_perms(Elf64_Word perms);

int main(int argc, char* argv[]) {
	FILE* stream;
	struct stat st;
	unsigned char* data;
	unsigned int datalen;

	if (argc != 2) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	if (stat(argv[1], &st) == -1) {
		perror("stat");
		return EXIT_FAILURE;
	}
	datalen = st.st_size;
	data = malloc(datalen);
	if (data == NULL) {
		fprintf(stderr, "Failed to malloc\n");
		return EXIT_FAILURE;
	}

	stream = fopen(argv[1], "rb");
	if (stream == NULL) {
		perror("fopen");
		free(data);
		return EXIT_FAILURE;
	}

	if (fread(data, 1, datalen, stream) != datalen) {
		fprintf(stderr, "Failed to read file data\n");
		free(data);
		return EXIT_FAILURE;
	}

	parse_elf(data, datalen);

	fclose(stream);
	free(data);

	return EXIT_SUCCESS;
}

int parse_elf(unsigned char* data, unsigned int datalen) {
    Elf64_Ehdr* ehdr;
    Elf64_Phdr* phdr;
	Elf64_Shdr* shdr;
	unsigned long int name_tbl_offset;
	unsigned int i;
	ehdr = (Elf64_Ehdr*)data;
	if (strncmp((char*)ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
		printf("Not a valid ELF file\n");
		return 1;
	}
	printf("Magic: %*s\n", SELFMAG, ELFMAG);
	printf("Type: "); print_object_file_type(ehdr->e_type); printf("\n");
	printf("Entry Point: %016lx\n", ehdr->e_entry);

	printf("\nSection headers:\n");
	shdr = (Elf64_Shdr*)(data + ehdr->e_shoff);
	name_tbl_offset = (shdr + ehdr->e_shstrndx)->sh_offset;
	printf("[Nr] %22s %10s %18s %8s %8s  %s %s %s %s\n",
		 "Name", "Type", "Address", "Off", "Size", "ES", "Lk", "Inf", "Al");
	for (i = 0; i < ehdr->e_shnum; i++, shdr++) {
		printf("[%02d] ", i);
		printf("%22s ", (char*)data + name_tbl_offset + shdr->sh_name);
		print_section_header_type(shdr->sh_type); printf(" ");
		printf("0x%016lx ", shdr->sh_addr);
		printf("0x%06lx ", shdr->sh_offset);
		printf("0x%06lx  ", shdr->sh_size);
		printf("%2lx  ", shdr->sh_entsize);
		printf("%2u ", shdr->sh_link);
		printf("%2u ", shdr->sh_info);
		printf("%02lu\n", shdr->sh_addralign);
	}

	printf("\nProgram headers:\n");
	printf("%16s %8s %18s %18s %8s %8s %3s %6s\n",
		 "Type", "Offset", "VirtAddr", "PhysAddr", "FileSiz", "MemSiz", "Flg", "Align");
	phdr = (Elf64_Phdr*)(data + ehdr->e_phoff);
	for (i = 0; i < ehdr->e_phnum; i++, phdr++) {
		print_program_header_type(phdr->p_type); printf(" ");
		printf("0x%06lx ", phdr->p_offset);
		printf("0x%016lx ", phdr->p_vaddr);
		printf("0x%016lx ", phdr->p_paddr);
		printf("0x%06lx ", phdr->p_filesz);
		printf("0x%06lx ", phdr->p_memsz);
		print_program_header_perms(phdr->p_flags); printf(" ");
		printf("0x%04lx\n", phdr->p_align);
	}
	
	return 0;
}

void print_program_header_perms(Elf64_Word perms) {
	if (PF_R & perms) {
		printf("R");
	}
	else {
		printf(" ");
	}
	if (PF_W & perms) {
		printf("W");
	}
	else {
		printf(" ");
	}
	if (PF_X & perms) {
		printf("E");
	}
	else {
		printf(" ");
	}
	return;
}

void print_program_header_type(Elf64_Word type) {
	char fmt[] = "%16s";
	switch (type) {
	case PT_NULL:
		printf(fmt, "PT_NULL");
		break;
	case PT_LOAD:
		printf(fmt, "PT_LOAD");
		break;
	case PT_DYNAMIC:
		printf(fmt, "PT_DYNAMIC");
		break;
	case PT_INTERP:
		printf(fmt, "PT_INTERP");
		break;
	case PT_NOTE:
		printf(fmt, "PT_NOTE");
		break;
	case PT_SHLIB:
		printf(fmt, "PT_SHLIB");
		break;
	case PT_PHDR:
		printf(fmt, "PT_PHDR");
		break;
	case PT_TLS:
		printf(fmt, "PT_TLS");
		break;
	case PT_NUM:
		printf(fmt, "PT_NUM");
		break;
	case PT_LOOS:
		printf(fmt, "PT_LOOS");
		break;
	case PT_GNU_EH_FRAME:
		printf(fmt, "PT_GNU_EH_FRAME");
		break;
	case PT_GNU_STACK:
		printf(fmt, "PT_GNU_STACK");
		break;
	case PT_GNU_RELRO:
		printf(fmt, "PT_GNU_RELRO");
		break;
	case PT_LOSUNW:
		printf(fmt, "PT_LOSUNW");
		break;
	case PT_SUNWSTACK:
		printf(fmt, "PT_SUNWSTACK");
		break;
	case PT_HISUNW:
		printf(fmt, "PT_HISUNW");
		break;
	case PT_LOPROC:
		printf(fmt, "PT_LOPROC");
		break;
	case PT_HIPROC:
		printf(fmt, "PT_HIPROC");
		break;
	default:
		printf(fmt, "Unknown");
		break;
	}
	return;
}

void print_section_header_type(Elf64_Word type) {
	char fmt[] = "%10s";
	switch (type) {
	case SHT_NULL:
		printf(fmt, "NULL");
		break;
	case SHT_PROGBITS:
		printf(fmt, "PROGBITS");
		break;
	case SHT_SYMTAB:
		printf(fmt, "SYMTAB");
		break;
	case SHT_STRTAB:
		printf(fmt, "STRTAB");
		break;
	case SHT_RELA:
		printf(fmt, "RELA");
		break;
	case SHT_HASH:
		printf(fmt, "HASH");
		break;
	case SHT_DYNAMIC:
		printf(fmt, "DYNAMIC");
		break;
	case SHT_NOTE:
		printf(fmt, "NOTE");
		break;
	case SHT_NOBITS:
		printf(fmt, "NOBITS");
		break;
	case SHT_REL:
		printf(fmt, "REL");
		break;
	case SHT_SHLIB:
		printf(fmt, "SHLIB");
		break;
	case SHT_DYNSYM:
		printf(fmt, "DYNSYM");
		break;
	case SHT_INIT_ARRAY:
		printf(fmt, "INIT_ARRAY");
		break;
	case SHT_FINI_ARRAY:
		printf(fmt, "FINI_ARRAY");
		break;
	default:
		printf(fmt, "Unknown");
		break;
	}
	return;
}

void print_object_file_type(Elf64_Half type) {
	switch(type) {
	case ET_NONE:
		printf("NONE");
		break;
	case ET_REL:
		printf("REL");
		break;
	case ET_EXEC:
		printf("EXEC");
		break;
	case ET_DYN:
		printf("DYN");
		break;
	case ET_CORE:
		printf("CORE");
		break;
	default:
		printf("Unknown");
		break;
	}
	return;
}

void print_usage(char* name) {
	printf("Usage: %s file\n", name);
}


