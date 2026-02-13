# Exercise 01 — Preprocessor Detective

**Difficulty:** ★☆☆ (Introductory)
**Type:** Original

## Concept

The C preprocessor is a text-substitution engine that runs *before* compilation.
It handles `#define` macros, `#include` directives, and conditional compilation
(`#ifdef`/`#endif`). Understanding what the preprocessor does — and what it
does *not* do — is essential to reading C code accurately.

**K&R Reference:** Section 4.11 (The C Preprocessor)

## Problem Statement

You are given a file (`starter.c`) containing several preprocessor macros:

- **Object-like macros:** `#define MAX_SIZE 100`
- **Function-like macros:** `#define SQUARE(x) ((x)*(x))`
- **Token pasting:** `#define CONCAT(a,b) a##b`
- **Stringification:** `#define STRINGIFY(x) #x`
- **Conditional compilation:** `#ifdef DEBUG` blocks
- **Predefined macros:** `__FILE__`, `__LINE__`, `__STDC_VERSION__`

Your task: fill in the `printf` statements so the program prints each macro's
expanded value. The goal is to *predict* what the preprocessor produces before
you compile and run.

## Hints

- Run `gcc -E starter.c` to see the preprocessed output (preprocessor only).
- `#x` turns the argument into a string literal.
- `a##b` concatenates two tokens into one identifier.
- `__STDC_VERSION__` reports the C standard version as a `long` integer.
- `#ifdef DEBUG` blocks are only compiled when `-DDEBUG` is passed to the compiler.

## Building & Testing

```bash
make            # Build starter and solution
make test       # Run tests against solution
make clean      # Remove binaries
gcc -E starter.c  # View preprocessor output (educational)
```
