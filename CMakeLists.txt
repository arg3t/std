# Project setup
cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)

project(YeetDbg VERSION 1.0)

include_directories(include ext/libelfin/ ext/linenoise/ src/yeetdbg)

add_executable(
  yeetdbg
    src/main.cxx
    ext/linenoise/linenoise.c
)

# Samples for debugging
add_executable(
  sleep_5
    samples/sleep.c
)

add_executable(
  dwarfer
    samples/dwarf_test.c
)

add_custom_target(
    libelfin
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin/
)

add_subdirectory(src/yeetdbg)

target_link_libraries(
  yeetdbg
  ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
  ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so
  Debugger
)

add_dependencies(yeetdbg libelfin)

# Sample executables autobuild

