# Exercise 03 — Separate Compilation

**Difficulty:** ★★★ (Advanced)
**Type:** Original (tests memory/linking understanding)

## Concept

Real C programs are split across multiple source files. Each `.c` file is
compiled independently into an object file (`.o`), and the linker combines
them into a single executable. This is **separate compilation** — a
fundamental part of the C compilation model.

To make separate compilation work, you need:

1. **Header files** (`.h`) — declare functions and shared variables with
   `extern`, so other files know they exist.
2. **Source files** (`.c`) — define (implement) functions and variables.
3. **The linker** — resolves cross-file references by matching symbols.

**K&R Reference:** Section 4.5 (Header Files), Section 4.6 (Static Variables)

## Problem Statement

You are given a monolithic single-file program (`starter.c`) that contains:

- `int factorial(int n)` — compute n!
- `int fibonacci(int n)` — compute the nth Fibonacci number
- `int call_count` — a global counter incremented by each function call
- `main()` — calls both functions and prints results

Your task: split this into three files:

| File | Contents |
|---|---|
| `solution.c` | `main()` — includes the header, calls helpers |
| `solution_helper.h` | Declarations: `extern int call_count;`, function prototypes |
| `solution_helper.c` | Definitions: `factorial()`, `fibonacci()`, `call_count` |

The output must match the original monolithic program exactly.

## Hints

- Use `extern int call_count;` in the header to declare (not define) the variable.
- Define `int call_count = 0;` in exactly one `.c` file.
- Use include guards (`#ifndef / #define / #endif`) in the header.
- Compile with: `gcc -c solution.c && gcc -c solution_helper.c && gcc -o solution solution.o solution_helper.o`
- Or in one step: `gcc solution.c solution_helper.c -o solution`

## Building & Testing

```bash
make            # Build starter and solution
make test       # Run tests against solution
make clean      # Remove binaries
```
