cmake_minimum_required(VERSION 3.10)

project(elf2pgz)

set (CMAKE_C_STANDARD 11)

if(MSVC)
  add_compile_options(/W3 /WX /wd4068 /J /D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1)
else(MSVC)
  add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter -funsigned-char)
endif(MSVC)

add_executable (elf2pgz
    main.c)

if(NOT MSVC)
    target_link_libraries (elf2pgz m)
endif(NOT MSVC)

install (TARGETS elf2pgz CONFIGURATIONS Release RUNTIME DESTINATION bin)
