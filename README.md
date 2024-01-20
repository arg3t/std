# SDB (The Simple Debugger)
This is a very simple practice project where I try to build a very basic CLI
compiler for ELF executables. It closesly follows the [tutorial](https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/)
by @TartanLLama.

## What does this do?
It is a very bare-bones debugger. It allows to you to set breakpoints, read and write registers and memory, step through instructions
and prints the disassembly starting from RIP. Sadly it does not support debugging hints in the format of DWARF because I could not find a
lightweight library for doing that and I am definitely not writing that from scratch. Still, it supports basic primitives for debugging.

## Why?


## Building and Running

