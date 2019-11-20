# GDB
~~~
# Set ENV var for programs
set env GUESSME=foobar

# Set Breakpoints
b *0xffffff

# Disassembly Main Entry point
disass main

# Display info on current stack frame
info frame

# Display sections of ELF
info files

~~~

# ReadELF
~~~
# File Header
readelf -h

# Program Headers
readelf -l

# Section Headers
readelf -S

~~~

# Objdump
~~~
objdump -sj .rodata <filename>
objdump -M intel -d <filename>

~~~