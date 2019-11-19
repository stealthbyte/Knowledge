## What are the differences between, Assembly File, Dissassm Object File, and Executable?
---
## Strip the binary and try to find functions again.
---
## Difference between code and data sections?
---
## Create Assembly
`gcc -S -masm=intel file_compile.c`

## Create Object File
`gcc -c file_assembly`

## Create stripped executeable
`gcc file_name -o file_name ; strip --strip-all file_name`

---
## Readelf/Objdump commands:
~~~
objdump -sj .rodata compilation_example.o

objdump -M intel -d compilation_example.o

readelf --relocs compilation_example.o

readelf --syms a.out
~~~