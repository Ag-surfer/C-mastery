# Exercise 02 — Object File Explorer

**Difficulty:** ★★☆ (Intermediate)
**Type:** Original (requires assembly/nm analysis)

## Concept

When you compile a `.c` file with `gcc -c`, the compiler produces a Mach-O
object file (`.o`). This file contains machine code, data, and a **symbol
table** that maps names to addresses and types. The `nm` tool reads the symbol
table and shows you what each name is: a function, a variable, an external
reference, etc.

Understanding symbol types is essential for debugging linker errors ("undefined
reference") and for understanding how separate compilation works.

## Problem Statement

You are given a file (`starter.c`) containing several declarations:

| Declaration | Storage | Symbol Type |
|---|---|---|
| `void global_function(void)` | global function | `T` (text/code) |
| `static void local_function(void)` | file-local function | `t` (local text) |
| `printf` | external (libc) | `U` (undefined) |
| `int global_initialized = 42` | initialized global | `D` (data) |
| `int global_uninitialized` | uninitialized global | `C` (common) |
| `static int static_var = 7` | file-local initialized | `d` (local data) |

Your task: fill in the `printf` statements predicting the correct `nm` symbol
type letter for each declaration. Run `nm -p solution.o` to verify.

## macOS nm Symbol Types

| Letter | Meaning |
|---|---|
| `T` / `t` | Text (code) segment — global / local |
| `D` / `d` | Data segment (initialized) — global / local |
| `S` / `s` | Other segment data — global / local |
| `C` | Common symbol (uninitialized global, resolved by linker) |
| `U` | Undefined (external, resolved at link time) |

Uppercase = global (visible to linker), lowercase = local (file-private).

## Hints

- Compile to an object file: `gcc -c -std=c11 solution.c -o solution.o`
- Inspect symbols: `nm -p solution.o`
- The `-p` flag shows symbols in order they appear (not sorted).
- `static` functions and variables get lowercase symbol types.

## Building & Testing

```bash
make            # Build starter and solution
make test       # Run tests against solution
make clean      # Remove binaries
nm -p solution.o  # Inspect symbol table (educational)
```
