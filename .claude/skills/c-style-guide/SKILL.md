---
name: c-style-guide
description: C-Mastery code style conventions. Auto-loads when editing any .c or .h file.
globs: ["**/*.c", "**/*.h"]
---

## File Header (REQUIRED on every .c file)
````c
/*
 * File: filename.c
 * Chapter: NN — Chapter Title
 * K&R Reference: §X.Y (section), Exercise X-Y (if applicable)
 * Concept: One-line description of what this file demonstrates
 *
 * Compile: gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g filename.c
 * Assembly: gcc -S -O0 -fno-asynchronous-unwind-tables filename.c
 * Sanitize: gcc -fsanitize=address,undefined -g filename.c
 * Debug: lldb ./filename
 * Note: gcc on macOS is Apple Clang 17. Assembly output is ARM64 on Apple Silicon.
 */
````

## Annotation Prefixes
- `// ASSEMBLY:` — explains what gcc -S produces for this line
- `// MEMORY:` — explains where this data lives (segment, offset, size)
- `// PITFALL:` — warns about undefined behavior or common bugs
- `// K&R:` — references specific K&R 2nd Edition section or exercise

## Code Conventions
- clang-format LLVM style, 4-space indent, 100-column limit
- C11 standard (`-std=c11`)
- All functions: return type on own line for declarations, same line for definitions ≤ 80 chars
- All parameters named (never `int func(int, char *)` — always `int func(int count, char *name)`)
- sizeof always with parentheses: `sizeof(int)`, never `sizeof int`
- NULL for null pointers, never 0 or (void *)0
- Every malloc has a matching free, commented with lifecycle

## Assembly Examples
When showing assembly, use this format (show YOUR platform's output):
````c
int x = 42;
// ASSEMBLY (ARM64 — Apple Silicon):
//   mov w8, #42          ; load immediate 42 into 32-bit register w8
//   str w8, [sp, #12]    ; store to stack at sp+12
// ASSEMBLY (x86-64 — for cross-reference):
//   movl $42, -4(%rbp)   ; store 42 at rbp-4
````

## Memory Layout Comments
````c
int arr[4] = {1, 2, 3, 4};
// MEMORY: stack segment, 16 bytes (4 × sizeof(int))
//   [sp+0]:  01 00 00 00  → arr[0] = 1
//   [sp+4]:  02 00 00 00  → arr[1] = 2
//   [sp+8]:  03 00 00 00  → arr[2] = 3
//   [sp+12]: 04 00 00 00  → arr[3] = 4
````

## Struct Documentation
Every struct must include:
````c
struct Example {
    int id;        // offset 0, size 4
    char grade;    // offset 4, size 1
    // 3 bytes padding (offset 5–7)
    double score;  // offset 8, size 8
};
// sizeof(struct Example) = 16, alignof = 8, padding = 3 bytes
````

## Printf Verification Pattern
After every struct definition, include:
````c
printf("sizeof(struct Example) = %zu\n", sizeof(struct Example));
printf("alignof(struct Example) = %zu\n", alignof(struct Example));
printf("offsetof(id) = %zu\n", offsetof(struct Example, id));
````

## Error Handling
- Check every return value from malloc, fopen, etc.
- Use `perror()` or `fprintf(stderr, ...)` for errors
- Return EXIT_FAILURE / EXIT_SUCCESS from main
- Never call `exit()` from library code — only from main

## Naming Conventions
- Functions: `snake_case`
- Variables: `snake_case`
- Constants/Macros: `UPPER_SNAKE_CASE`
- Types (typedef): `PascalCase_t` or `snake_case_t`
- File names: `snake_case.c`
