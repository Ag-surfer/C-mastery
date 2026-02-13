---
name: assembly-annotation
description: gcc -S output annotation conventions and instruction reference. Auto-loads when working with assembly.
globs: ["**/*.s", "**/*asm*"]
---

## Generating Assembly
````bash
# Apple Clang 17 (your MacBook Pro — gcc is aliased to clang)
gcc -S -O0 -fno-asynchronous-unwind-tables -masm=att filename.c -o filename.s
# -O0: no optimization (matches source line-by-line)
# -fno-asynchronous-unwind-tables: removes .cfi directives (cleaner output)
# -masm=att: AT&T syntax (default on macOS)
#
# On Apple Silicon (ARM64): assembly will be AArch64, not x86-64
# Key differences: registers are x0-x30 (not %rax-%r15), instructions differ
# To get x86-64 output on Apple Silicon (cross-compile):
#   gcc -S -O0 -target x86_64-apple-macos filename.c -o filename_x86.s
````

## Annotation Format — ARM64 (Apple Silicon, your native output)
````asm
_main:
    stp     x29, x30, [sp, #-16]!  ; Save frame pointer + link register (push pair)
    mov     x29, sp                 ; Set up new frame pointer
    sub     sp, sp, #16             ; Allocate 16 bytes on stack
    mov     w8, #42                 ; int x = 42; (load immediate into 32-bit reg)
    str     w8, [sp, #12]          ; Store x on stack at sp+12
    adrp    x0, l_.str@PAGE         ; Load format string page address
    add     x0, x0, l_.str@PAGEOFF  ; Add page offset (full address in x0 = arg1)
    ldr     w1, [sp, #12]          ; Load x from stack into w1 (arg2 for printf)
    bl      _printf                 ; Branch-with-link to printf (return addr in x30)
    mov     w0, #0                  ; Return value 0
    ldp     x29, x30, [sp], #16    ; Restore frame pointer + link register (pop pair)
    ret                             ; Return to caller (branch to x30)
````

## Annotation Format — x86-64 (cross-compile or Intel reference)
````asm
main:
    pushq   %rbp              # Save caller's frame pointer
    movq    %rsp, %rbp        # Set up new frame pointer
    subq    $16, %rsp         # Allocate 16 bytes on stack (alignment)
    movl    $42, -4(%rbp)     # int x = 42; (4 bytes at rbp-4)
    movl    -4(%rbp), %esi    # Load x into arg2 register for printf
    leaq    .LC0(%rip), %rdi  # Load format string address into arg1
    call    printf@PLT        # Call printf via PLT (dynamic linking)
    movl    $0, %eax          # Return value 0
    leave                     # Restore frame pointer (movq %rbp,%rsp; popq %rbp)
    ret                       # Return to caller
````

## Key x86-64 Registers (cross-compile target or Intel Macs)
| Register | Purpose | Calling Convention |
|---|---|---|
| %rax | Return value | Caller-saved |
| %rdi | 1st integer argument | Caller-saved |
| %rsi | 2nd integer argument | Caller-saved |
| %rdx | 3rd integer argument | Caller-saved |
| %rcx | 4th integer argument | Caller-saved |
| %r8  | 5th integer argument | Caller-saved |
| %r9  | 6th integer argument | Caller-saved |
| %rbp | Frame pointer | Callee-saved |
| %rsp | Stack pointer | Callee-saved |

## Key ARM64 Registers (Apple Silicon — your native architecture)
| Register | Purpose | Calling Convention |
|---|---|---|
| x0 | 1st argument AND return value | Caller-saved |
| x1 | 2nd argument | Caller-saved |
| x2–x7 | 3rd–8th arguments | Caller-saved |
| x8 | Indirect result location | Caller-saved |
| x29 (fp) | Frame pointer | Callee-saved |
| x30 (lr) | Link register (return address) | Callee-saved |
| sp | Stack pointer | Special |
| xzr | Zero register (reads as 0, writes discarded) | — |

## Instruction Suffixes (x86-64)
- `b` = byte (1 byte, char)
- `w` = word (2 bytes, short)
- `l` = long (4 bytes, int)
- `q` = quad (8 bytes, long/pointer on 64-bit)

## ARM64 Instruction Width (Apple Silicon)
- `b` suffix or `w0–w30` = 32-bit operation (int)
- No suffix or `x0–x30` = 64-bit operation (long/pointer)
- `str`/`ldr` = store/load (equivalent to x86 mov)
- `stp`/`ldp` = store/load pair (two registers at once, common in prologues)

## Common Patterns — x86-64
- `leaq addr, %reg` — Load Effective Address (compute address, NO memory access)
- `movl val, addr` — Move 32-bit value (DOES access memory)
- `pushq %reg` — Push 8 bytes onto stack (decrements %rsp by 8)
- `call func@PLT` — Call via Procedure Linkage Table (dynamic linking)
- `leave` — Equivalent to `movq %rbp, %rsp; popq %rbp`

## Common Patterns — ARM64 (Apple Silicon)
- `adr x0, label` — Compute address of label (like LEA, no memory access)
- `ldr w0, [sp, #12]` — Load 32-bit value from stack (like movl from memory)
- `str w0, [sp, #12]` — Store 32-bit value to stack (like movl to memory)
- `stp x29, x30, [sp, #-16]!` — Push frame pointer + link register (function prologue)
- `ldp x29, x30, [sp], #16` — Pop frame pointer + link register (function epilogue)
- `bl _printf` — Branch with link (like call — stores return address in x30)
- `ret` — Return (branches to address in x30/lr)
