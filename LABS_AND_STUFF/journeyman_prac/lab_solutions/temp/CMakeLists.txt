cmake_minimum_required(VERSION 3.14)



add_executable( mem memory.c )
target_link_libraries( mem rt )





# template commands

# EXECUTABLE BASIC
# add_executable( <target> <file1.c> )

# MAKE DYNAMIC LOAD SHARED OBJECT
# add_library( <lib_name> SHARED <lib_file1.c> )
# set_target_properties( <lib_name> PROPERTIES COMPILE_FLAGS "-fPIC" )

# MAKE STATIC LIBRARY FOR LINKING
# add_library( <lib_name> STATIC <lib_file1.c> )
# add_custom_command( bad_math POST_BUILD COMMAND cp ARGS build/bad_math.so /var/ )

# STATIC EXECUTABLE
# add_executable( <bin> <file1.c> )
# target_link_libraries( <bin> <lib_static> )

# EXECUTABLE w/auto-load dynamic 
# add_executable( <bin> <file1.c> )
# target_link_libraries( 4_auto_dynamic.elf bad_math )

# EXECUTABLE w/manual laod dynamic
# add_executable( <target> <file1.c> )
# target_link_libraries( <target> dl )
# set_target_properties( <target> PROPERTIES COMPILE_FLAGS "-L -Wl,-rpath=." )

#debug symbols
# set_target_properties( <target> PROPERTIES COMPILE_FLAGS "-g" )