# Project setup
cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)

project(std VERSION 1.0)

include_directories(
  include
  ext/libelfin/
  ext/linenoise/
  ext/zydis/include/
  ext/zydis/dependencies/zycore/include/
  src/std
)

add_custom_target(
    libelfin
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin/
)

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory(ext/zydis)

add_subdirectory(src/std)

add_executable(
  std
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

target_link_libraries(
  std PRIVATE
  Debugger
  ${PROJECT_BINARY_DIR}/ext/zydis/libZydis.a
  ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
  ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so
)
add_dependencies(std libelfin)

# Sample executables autobuild

