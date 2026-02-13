# Chapter 01: Hello Machine

## Overview

Every C programmer writes "Hello, World!" as their first program. Most stop at "it prints text." This chapter does not stop there. We trace every stage of `hello.c` from source code to running process: what the preprocessor does with `#include`, how the compiler translates `printf` into ARM64 instructions, where the string `"Hello, World!\n"` lives in memory, and what happens after `main()` returns. By the end of this chapter, you will understand that a C program is not an abstract script -- it is a precise description of what the machine should do with its registers, memory, and CPU.

## K&R Reference

- **Sections:** 1.1 "Getting Started", 1.2 "Variables and Arithmetic Expressions" (escape sequences)
- **Exercises:** 1-1 (experimenting with errors), 1-2 (escape sequences)
- **Key Quote:** "The only way to learn a new programming language is by writing programs in it. The first program to write is the same for all languages: print the words hello, world." -- K&R, Section 1.1

## Layer 1: Concept

The simplest C program has three essential parts:

```c
#include <stdio.h>      // 1. Preprocessor directive

int main(void) {         // 2. Entry point
    printf("Hello, World!\n");  // 3. Library function call
    return 0;            // 4. Exit status
}
```

Each part maps to a specific stage in the compilation pipeline and a specific region of process memory:

**`#include <stdio.h>`** is a preprocessor directive. Before compilation begins, the preprocessor replaces this line with the entire contents of `stdio.h` (roughly 700 lines on macOS). This provides the declaration of `printf()`. Without it, the compiler has no way to verify the function signature and will emit an error under `-Werror`.

**`int main(void)`** declares the program entry point. The C11 standard (5.1.2.2.1) specifies two valid forms: `int main(void)` and `int main(int argc, char *argv[])`. The return type `int` is the exit status: 0 means success, non-zero means failure. The shell stores this value in `$?`.

**`printf("Hello, World!\n")`** is a call to a variadic function in the C standard library. The string literal `"Hello, World!\n"` occupies 15 bytes in memory (14 characters plus the NUL terminator `\0`). The `\n` is an escape sequence -- a single byte with value 10 (0x0A), the newline character.

**`return 0`** ends `main()` and passes the exit status to the C runtime, which calls `atexit` handlers, flushes stdio buffers, and issues the `_exit()` system call.

### The Compilation Pipeline

A C source file does not become an executable in one step. There are four distinct stages, each producing a different intermediate file:

| Stage | Tool | Input | Output | Flag |
|-------|------|-------|--------|------|
| Preprocessing | cpp | `hello.c` | `hello.i` | `gcc -E` |
| Compilation | cc1 | `hello.i` | `hello.s` | `gcc -S` |
| Assembly | as | `hello.s` | `hello.o` | `gcc -c` |
| Linking | ld | `hello.o` | `a.out` | `gcc` (default) |

Try it yourself:

```bash
gcc -E hello.c -o hello.i    # Preprocessor output (~700 lines)
gcc -S -O0 hello.c           # Assembly output (ARM64)
gcc -c hello.c               # Object file (Mach-O relocatable)
gcc hello.c -o hello         # Final executable (Mach-O linked)
```

See [Diagram 1: The Compilation Pipeline](#layer-4-diagrams) for a visual representation.

### The Startup Sequence

`main()` is not the first thing that runs. When you execute `./hello`, the operating system and C runtime perform significant setup before your code executes:

1. The kernel creates a new process and loads the Mach-O binary into memory.
2. `dyld` (the dynamic linker) maps shared libraries (`libSystem.dylib`, which contains libc) into the process address space.
3. `dyld` calls library initializers (C++ constructors, Objective-C `+load` methods).
4. `dyld` transfers control to the C runtime entry point.
5. The C runtime startup code calls `main(argc, argv)`.

This is explored in detail in `examples/hello_anatomy.c`.

## Layer 2: Assembly Translation

When the compiler translates `hello_asm.c` with `gcc -S -O0 -fno-asynchronous-unwind-tables`, it produces the following ARM64 assembly. Every instruction is annotated with its purpose.

```asm
    .section  __TEXT,__text,regular,pure_instructions
    .globl    _main
    .p2align  2
    _main:
      ;; === PROLOGUE: set up the stack frame ===
      sub   sp, sp, #32             ; allocate 32 bytes on the stack
      stp   x29, x30, [sp, #16]    ; save frame pointer (x29) + link register (x30)
      add   x29, sp, #16           ; establish our frame pointer

      ;; === PREPARE RETURN VALUE ===
      mov   w8, #0                  ; w8 = 0 (the return value)
      str   w8, [sp, #8]           ; spill return value to stack at [sp+8]
      stur  wzr, [x29, #-4]        ; store 0 at implicit return slot [x29-4]

      ;; === CALL puts("Hello") ===
      adrp  x0, l_.str@PAGE        ; load page address of string literal
      add   x0, x0, l_.str@PAGEOFF ; add page offset -> x0 = &"Hello"
      bl    _puts                   ; call puts(x0) -- writes "Hello\n" to stdout

      ;; === EPILOGUE: tear down stack frame and return ===
      ldr   w0, [sp, #8]           ; reload return value 0 into w0
      ldp   x29, x30, [sp, #16]   ; restore frame pointer + link register
      add   sp, sp, #32           ; deallocate stack frame
      ret                          ; return to caller via x30 (link register)

    ;; === READ-ONLY STRING DATA ===
    .section  __TEXT,__cstring,cstring_literals
    l_.str:
      .asciz  "Hello"              ; 6 bytes: 'H','e','l','l','o','\0'
```

### Key ARM64 Concepts

**Registers used:**
- `sp` -- Stack pointer. Always 16-byte aligned on ARM64.
- `x29` -- Frame pointer. Points into the current stack frame. Debuggers (lldb) follow the x29 chain to walk the call stack.
- `x30` -- Link register. Holds the return address. `bl` (branch with link) saves the return address here before jumping.
- `x0` -- First argument register (AAPCS64 calling convention). Used to pass the string pointer to `puts()`.
- `w0` -- Lower 32 bits of `x0`. Used for the `int` return value of `main()`.
- `w8`, `wzr` -- `w8` is a temporary register. `wzr` is the zero register (always reads as 0).

**Prologue / Epilogue pattern:** Every function begins with a prologue (allocate stack, save registers) and ends with an epilogue (restore registers, deallocate stack, `ret`). This pattern is universal across all compiled C functions.

**`adrp` + `add` for string loading:** ARM64 uses PC-relative page addressing because immediates are limited to 21 bits. `adrp` loads the page-aligned base address; `add` adds the offset within that page. Together they produce the full address of the string literal.

## Layer 3: Memory Layout

When `hello` runs, the OS creates a virtual address space divided into segments. Each variable, string literal, and instruction lives at a specific address in a specific segment.

### Stack Frame for `_main` (from `hello_asm.c`)

```
  High addresses (stack grows downward)
  +----------------------------+
  | [sp+24]: saved x30 (LR)   |  8 bytes -- return address
  | [sp+16]: saved x29 (FP)   |  8 bytes -- previous frame pointer
  | [sp+12]: implicit return   |  4 bytes -- stur wzr, [x29, #-4]
  | [sp+8]:  return val spill  |  4 bytes -- str w8 (reload source)
  | [sp+0]:  (unused/padding)  |  8 bytes -- alignment padding
  +----------------------------+
  Low addresses
  Total: 32 bytes (16-byte aligned as required by ARM64 ABI)
```

### Process Address Space

```
  +--------------------------+  0x16FDFF000  (high addresses)
  |         Stack            |  local variables, return addresses
  |       grows down         |  argc, argv, saved registers
  +--------------------------+
  |           |              |
  |     (unmapped gap)       |  grows toward each other
  |           |              |
  +--------------------------+
  |         Heap             |  malloc'd memory
  |       grows up           |  (not used in hello.c)
  +--------------------------+
  |         BSS              |  uninitialized globals (zeroed)
  +--------------------------+
  |         Data             |  initialized globals
  +--------------------------+
  |   ROData / __cstring     |  "Hello, World!\n" (15 bytes)
  +--------------------------+
  |   Text / __text          |  main() machine code
  +--------------------------+  0x100003F00  (low addresses)
```

The addresses are from an actual Apple Silicon (ARM64) macOS process. You can verify this yourself by running `examples/hello.c`, which prints the addresses of `main()` and the string literal. Exercise 03 (Process Memory Explorer) explores all six segments systematically.

See [Diagram 2: Process Memory Map](#layer-4-diagrams) for the visual representation.

## Layer 4: Diagrams

### Diagram 1: The Compilation Pipeline

![Compilation Pipeline](diagrams/ch01-compilation-pipeline.excalidraw)

This diagram shows the four-stage transformation from source code to executable. Each stage is a separate program (preprocessor, compiler, assembler, linker) that produces a distinct intermediate file. The `gcc` command is actually a driver that invokes each stage in sequence. You can stop at any stage using the flags shown (`-E`, `-S`, `-c`).

### Diagram 2: Process Memory Map

![Process Memory Map](diagrams/ch01-process-memory-map.excalidraw)

This diagram shows the six segments of a running C process. High addresses (stack) are at the top; low addresses (text/code) are at the bottom. The stack grows downward and the heap grows upward. For `hello.c`, only three segments are actively used: Text (the `main()` machine code), ROData (the `"Hello, World!\n"` string literal), and Stack (the 32-byte stack frame for `main`).

## Layer 5: Pitfalls & Undefined Behavior

### Pitfall 1: Missing `#include`

Removing `#include <stdio.h>` means the compiler has no declaration for `printf()`. Under `-Werror`, this is a hard error. Without `-Werror`, older compilers might assume `printf` returns `int` and takes whatever arguments you pass -- this implicit declaration was removed in C99 and is explicitly forbidden in C11.

### Pitfall 2: Modifying a String Literal

```c
char *msg = "Hello";
msg[0] = 'h';  // UNDEFINED BEHAVIOR
```

String literals live in the read-only `__TEXT,__cstring` segment. The OS maps this memory as read-only. On most systems, writing to it causes a segmentation fault (SIGSEGV). On some systems, it might silently "work" because the memory happens to be writable -- but the behavior is undefined. AddressSanitizer does not catch this (it is an OS-level protection), but the crash is immediate on macOS.

### Pitfall 3: Forgetting `return 0`

In C99 and C11, reaching the end of `main()` without a `return` statement is equivalent to `return 0`. However, this is unique to `main()` -- in any other function with a non-void return type, falling off the end without returning a value is undefined behavior. Always be explicit.

### Pitfall 4: Assuming `printf` Return Value

`printf` returns the number of characters printed, or a negative value on error. Most programmers ignore this return value, which is fine in practice -- but in safety-critical code, checking the return value of every I/O operation is required.

### Detecting Issues

```bash
# Compile with all warnings as errors
gcc -Wall -Wextra -Werror -std=c11 -pedantic hello.c

# Run with AddressSanitizer and UBSan
gcc -fsanitize=address,undefined -g hello.c -o hello && ./hello
```

Exercise 01 (Break Hello World) systematically explores these pitfalls by deliberately introducing each error and identifying which compilation stage catches it.

## Examples

### `examples/hello.c` -- The Annotated Hello World

The classic "Hello, World!" program with every line annotated. This file demonstrates:

- The preprocessor directive `#include <stdio.h>` and what it expands to (~700 lines)
- The `main()` entry point and its relationship to the linker symbol `_main`
- The string literal `"Hello, World!\n"` and where it lives in memory (`__TEXT,__cstring`)
- Runtime verification: printing `sizeof` the string literal, the address of the string, and the address of `main()`

Key observation: the string literal address and `main()` address are both in the low region of the address space (around `0x100003xxx` on ARM64 macOS), because they both live in the `__TEXT` segment.

### `examples/hello_anatomy.c` -- Anatomy of main()

Goes deeper into what happens before and after `main()`:

- The full `main(int argc, char *argv[])` signature with `argc`/`argv` exploration
- The startup sequence: kernel, `dyld`, C runtime, then `main()`
- The shutdown sequence: `atexit` handlers, stdio flush, `_exit()` syscall
- Stack frame layout with ARM64 register spills: `argc` at `[x29-8]`, `argv` at `[x29-16]`

Run with arguments to see `argv` in action:

```bash
./hello_anatomy arg1 arg2 arg3
```

### `examples/hello_asm.c` -- Assembly-Focused Hello

A minimal program designed for clean assembly analysis. Uses `puts()` instead of `printf()` because `puts` takes a single string argument (not variadic), producing simpler assembly. The complete ARM64 assembly output is reproduced in the file header with line-by-line explanation of every instruction, including the full 32-byte stack frame layout.

Generate the assembly yourself:

```bash
gcc -S -O0 -fno-asynchronous-unwind-tables hello_asm.c
cat hello_asm.s
```

## Exercises

| # | Title | Difficulty | Source |
|---|-------|-----------|--------|
| 1 | [Break Hello World](exercises/exercise_01/) | :star: | Adapted from K&R Exercise 1-1 |
| 2 | [Escape Sequence Lab](exercises/exercise_02/) | :star::star: | Adapted from K&R Exercise 1-2 |
| 3 | [Process Memory Explorer](exercises/exercise_03/) | :star::star::star: | Original |

**Exercise 01** asks you to deliberately break `hello.c` in four different ways and identify which stage of the compilation pipeline catches each error. This connects directly to the compilation pipeline diagram and Layer 1 concepts.

**Exercise 02** explores escape sequences as byte values. Every escape sequence (`\n`, `\t`, `\\`, `\"`, `\0`, `\a`, `\b`, `\r`) maps to a specific numeric value. You build a formatted table showing the escape notation, the character it produces, and its decimal/hexadecimal value. This reinforces that characters in C are numbers.

**Exercise 03** makes the invisible memory layout visible. You declare variables in each segment (text, rodata, data, BSS, heap, stack) and print their addresses to verify the memory map from Layer 3. This is the exercise that transforms the process memory diagram from an abstract picture into something you can observe at runtime.

## Going Beyond K&R

K&R was published in 1988. Several tools and concepts critical to modern C were not available:

**AddressSanitizer (`-fsanitize=address`):** Detects out-of-bounds memory access, use-after-free, and double-free at runtime. Available in Clang and GCC since ~2012. Every example in this curriculum is compiled and tested with ASan enabled.

**Undefined Behavior Sanitizer (`-fsanitize=undefined`):** Detects signed integer overflow, null pointer dereferences, misaligned accesses, and other undefined behaviors at runtime. K&R-era C relied on programmers to avoid UB manually.

**`lldb` debugger:** On macOS, `lldb` replaces `gdb`. You can examine memory, registers, and the call stack:

```bash
lldb ./hello
(lldb) breakpoint set --name main
(lldb) run
(lldb) register read x0 x29 x30 sp
(lldb) memory read --size 1 --count 15 --format c $x0
```

**C11 standard guarantees:** The C11 standard (ISO/IEC 9899:2011) formalized behaviors that were implementation-defined in K&R's era, including the two valid forms of `main()`, the guarantee that `argv[argc] == NULL`, and `_Static_assert` for compile-time checks.

**Modern compiler warnings:** The flags `-Wall -Wextra -Werror -pedantic` catch dozens of common errors that K&R-era compilers would silently accept. Treat warnings as errors from day one.

## Summary

After this chapter, you should understand:

1. **A C source file is not a program.** It is a description that passes through four stages (preprocessor, compiler, assembler, linker) before becoming an executable.
2. **Every C statement maps to specific machine instructions.** `printf("Hello")` becomes `adrp` + `add` (load string address) + `bl` (call function). `return 0` becomes `ldr w0` (load return value) + `ldp` + `add sp` (restore stack) + `ret`.
3. **Every variable and string has a specific address in memory.** String literals live in `__TEXT,__cstring` (read-only). Local variables live on the stack. The `main()` function code lives in `__TEXT,__text`.
4. **`main()` is not the first or last thing that runs.** The OS, dynamic linker, and C runtime set up the process before `main()` and tear it down after `main()` returns.
5. **The compiler is your first line of defense.** Using `-Wall -Wextra -Werror -std=c11 -pedantic` catches errors that would otherwise become runtime bugs or undefined behavior.

Build and run everything:

```bash
cd chapters/01-hello-machine
make          # compile all examples with ASan + UBSan
make asm      # generate assembly output
make test     # run all exercise test harnesses
```
