/*
 * hello_asm.c -- Chapter 01: Hello Machine
 * K&R 2nd Ed. Section 1.1 -- "Getting Started"
 * Concept: A minimal hello world designed for clean assembly analysis.
 *          Compile with: gcc -S -O0 -fno-asynchronous-unwind-tables hello_asm.c
 *          to see the ARM64 assembly this program generates.
 *
 * Why puts() instead of printf()?
 *   puts() takes a single string argument and appends a newline.
 *   printf() is variadic (variable arguments) and requires format
 *   string parsing at runtime.  Using puts() produces simpler assembly
 *   that is easier to read and annotate for learning purposes.
 *
 * Compile: gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *          hello_asm.c -o hello_asm
 * Assembly: gcc -S -O0 -fno-asynchronous-unwind-tables hello_asm.c
 */

#include <stdio.h>

/*
 * ACTUAL ARM64 ASSEMBLY OUTPUT (gcc -S -O0 -fno-asynchronous-unwind-tables):
 * ===========================================================================
 * The program has two sections in the Mach-O binary:
 *
 *   __TEXT,__text     -- executable machine code (main function)
 *   __TEXT,__cstring  -- read-only string constants ("Hello")
 *
 * ASSEMBLY (actual Apple Clang 17 output on ARM64 macOS):
 *
 *   .section  __TEXT,__text,regular,pure_instructions
 *   .globl    _main
 *   .p2align  2
 *   _main:
 *     sub   sp, sp, #32                    ; allocate 32 bytes of stack
 *     stp   x29, x30, [sp, #16]           ; save frame pointer + link register
 *     add   x29, sp, #16                  ; set frame pointer to sp+16
 *     mov   w8, #0                        ; prepare return value 0
 *     str   w8, [sp, #8]                  ; spill return value to stack
 *     stur  wzr, [x29, #-4]              ; store 0 at [x29-4] (implicit return
 * slot) adrp  x0, l_.str@PAGE              ; load page address of string
 * literal add   x0, x0, l_.str@PAGEOFF       ; add page offset -> x0 = &"Hello"
 *     bl    _puts                         ; call puts(x0)
 *     ldr   w0, [sp, #8]                 ; reload return value 0 into w0
 *     ldp   x29, x30, [sp, #16]          ; restore frame pointer + link
 * register add   sp, sp, #32                  ; deallocate stack frame ret ;
 * return to caller
 *
 *   .section  __TEXT,__cstring,cstring_literals
 *   l_.str:
 *     .asciz  "Hello"
 *
 * LINE-BY-LINE EXPLANATION:
 *
 *   sub   sp, sp, #32
 *     Allocate 32 bytes on the stack.  ARM64 requires 16-byte aligned sp.
 *     The compiler allocates more space than strictly needed at -O0 for
 *     debug convenience and spill slots.
 *
 *   stp   x29, x30, [sp, #16]
 *     Store Pair: save frame pointer (x29) and link register (x30)
 *     at sp+16.  x30 holds the return address (where to go after ret).
 *     This is part of the function PROLOGUE -- saving the caller's context.
 *
 *   add   x29, sp, #16
 *     Set our frame pointer to sp+16 (where we just saved the old x29).
 *     Debuggers (lldb) use the x29 chain to walk the call stack.
 *
 *   mov   w8, #0  /  str w8, [sp, #8]
 *     At -O0, Clang often pre-computes the return value and spills it
 *     to the stack.  This is an optimization-disabled artifact; at -O2
 *     the compiler would just use "mov w0, #0" right before ret.
 *
 *   stur  wzr, [x29, #-4]
 *     Store zero to the implicit return value slot.  wzr is the
 *     "zero register" -- a hardware register that always reads as 0.
 *     stur is "store unscaled" (for offsets not aligned to 4 bytes).
 *
 *   adrp  x0, l_.str@PAGE
 *     Address of Page: load the 4KB-aligned page address containing
 *     our string literal into x0.  ARM64 uses PC-relative page
 *     addressing because immediates are limited to 21 bits.
 *
 *   add   x0, x0, l_.str@PAGEOFF
 *     Add the offset within the page to get the exact address of
 *     "Hello".  Now x0 = pointer to the string.  x0 is the first
 *     argument register in the ARM64 calling convention (AAPCS64).
 *
 *   bl    _puts
 *     Branch with Link: call puts().  'bl' saves the return address
 *     in x30 (link register) and jumps to _puts in libSystem.
 *     puts() writes "Hello\n" to stdout (file descriptor 1).
 *
 *   ldr   w0, [sp, #8]
 *     Reload the return value (0) from the spill slot into w0.
 *
 *   ldp   x29, x30, [sp, #16]
 *     Load Pair: restore frame pointer and link register from stack.
 *     This is the function EPILOGUE -- restoring the caller's context.
 *
 *   add   sp, sp, #32
 *     Deallocate the stack frame (undo the sub at the start).
 *
 *   ret
 *     Return: jump to the address in x30 (link register).
 *     Control returns to the C runtime, which calls _exit(0).
 *
 * MEMORY MAP (process address space):
 *
 *   +---------------------------+  High addresses (stack grows down)
 *   | Stack (32 bytes total)    |
 *   |  [sp+24]: saved x30 (LR) |  <- 8 bytes: return address
 *   |  [sp+16]: saved x29 (FP) |  <- 8 bytes: frame pointer
 *   |  [sp+12]: implicit return |  <- 4 bytes: stur wzr, [x29, #-4]
 *   |  [sp+8]:  return val spill|  <- 4 bytes: str w8 (reload source)
 *   |  [sp+0]:  (unused/pad)   |  <- 8 bytes: alignment padding
 *   +---------------------------+
 *   |         ...               |
 *   +---------------------------+
 *   | __TEXT,__cstring          |  <- "Hello\0" (6 bytes, read-only)
 *   +---------------------------+
 *   | __TEXT,__text             |  <- main() machine code (read+execute)
 *   +---------------------------+  Low addresses
 */
int main(void) {
  // MEMORY: "Hello" is stored in __TEXT,__cstring (6 bytes:
  // 'H','e','l','l','o','\0') ASSEMBLY: adrp + add to load string address into
  // x0, then bl _puts
  puts("Hello");

  // ASSEMBLY: ldr w0, [sp, #8]              ; reload return value 0 from spill
  // ASSEMBLY: ldp x29, x30, [sp, #16]      ; epilogue: restore FP + LR
  // ASSEMBLY: add sp, sp, #32              ; deallocate stack frame
  // ASSEMBLY: ret                          ; return to caller via x30
  return 0;
}
