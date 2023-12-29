# Project setup
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
project(YeetDbg VERSION 1.0)

include_directories(include ext/libelfin/ ext/linenoise/)

add_executable(yeetdbg src/yeetdbg.cxx ext/linenoise/linenoise.c)

add_custom_target(
    libelfin
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin/
)

target_link_libraries(
  yeetdbg
  ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
  ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so
)

add_dependencies(yeetdbg libelfin)

# Sample executables autobuild

