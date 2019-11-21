#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>


void *read_elf(char *elfName, char *section)
{
        size_t fileSize;
        Elf64_Ehdr elfHeader;
        Elf64_Shdr sectHdr;
        
        FILE* file=fopen("hello", "rb");
        
        // FIND FILE SIZE and copy into elfStruct
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        
        // Set back to normal
        fseek(file, 0, SEEK_SET);
        printf("Size of file %d\n", fileSize);
        
        // Allocate memory on heap to store elf
        char *buffer = (unsigned char *) malloc(fileSize);
        
        // Copy elf contents into buffer
        fread(buffer, 1, fileSize, file);
        
        // Extract Elf Header from Buffer
        memcpy(&elfHeader, buffer, 64);
        
        // Verify contents in buffer by printing the Magic
        printf("File is --> %s\n", elfHeader.e_ident);

        // Calculate size of elf to find aprop offset
        unsigned long elfSize;
        FILE * ElfFile = NULL;
        /**
         * We know that the section header is the last part of the ELF
         * The e_shoff is the offset of the section header in the ELF
         * So all you need to do next is calculate the number of sections * size of each section
         * Add those values together and you have the size of your ELF binary
         * */

        elfSize = elfHeader.e_shoff + (elfHeader.e_shnum * elfHeader.e_shentsize);
        printf("Calculated size: %d\n", elfSize);

        // Section Header
        

}

int main()
{
    read_elf("hello", ".text");
}

