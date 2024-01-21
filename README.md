# STD (Simple Terminal Debugger)
This is a very simple practice project where I try to build a very basic CLI
compiler for ELF executables. It closesly follows the [tutorial](https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/)
by @TartanLLama.

## What does this do?
It is a very bare-bones debugger. It allows to you to set breakpoints, read and write registers and memory, step through instructions
and prints the disassembly starting from RIP. Sadly it does not support debugging hints in the format of DWARF because I could not find a
lightweight library for doing that and I am definitely not writing that from scratch. Still, it supports basic primitives for debugging.

## Why?

Just a hobby project. I had some time one my hands.

I am aware that it is not on par with debuggers like gdb. But I enjoyed writing it and might continue adding stuff on top if I feel like it.


## Features

Supports:
- Memory Read/Write
- Registers Read/Write
- Breakpoints
- Disassembly

Does not support:
- Debug symbols by parsing DWARF data. I could not find a nice and simple DWARF parsing library that support DWARFv5. Might write my own eventually.


## Building and Running

Create build directory after cloning and run cmake in it:

```
mkdir build; cd build
cmake ..
cmake --build .
```

Now you can start std by running

```
./std sleep_5
```

Running `help` when you are dropped into the std command line should give you a list of commands. Enjoy.
