# Useful Commands
## Look for embedded ELF files using grep  
`grep 'ELF' *`  
Next you find an embedded ELF file, how do you extract it? How long is it?
First extract the header, so you can calculate the size of the ELF payload.  
## Look where the ELF Magic Byte starts in embedded file

`xxd <file> | head -n 20`

## Find the Hexadecimal offset where *ELF* is and extract exactly 64 bytes after that. 64bit ELF have exactly 64 bytes for Headers.

`dd skip=52 count=64 if=<file> of=elf_header bs=1`

## Use a utility to read ELF header and calculate size
`readelf -h elf_header`  
~~~
First: (Start of section headers)
  - 8568 (bytes into file)  

Second: (Size of section headers)
  - 64 (bytes)

Lastly: (Number of section headers)
  - 27

Final Size of ELF:
8568 + (27 X 64) = 10,296 (bytes)
~~~

## Extract embedded ELF and configure path to load it
`dd skip=52 count=10296 if=67b8601 of=lib5ae9b7f.so bs=1`  
`export LD_LIBRARY_PATH=`\`pwd`  

## Investigate new ELF binary, find some interesting syscalls to mangled functions.. Can we clean this up?
`readelf -sh <binary>.so`  
`nm --demangle --dynamic <binary>.so`  
or  
`c++filt <mangled_function_name>`

## Execute Binary... Nothing exciting happens... time for more static/dynamic analysis
### Static:
---
`strings <binary>`  
### Dynamic:
---
`strace ./<binary> testing_cmd_args`  
`ltrace -i -C ./<binary> testing_cmd_args`  
#### Set Breakpoints where needed while running GDB. The program does a comparision at runtime, you need to dump the memory right before comparison.
`gdb ./<binary>`  
`set env GUESSME=customVarforBinary`
