# Exercise 01: Break Hello World

**Difficulty:** :star:

**Adapted from K&R Exercise 1-1:** "Run the 'hello, world' program on your system.
Experiment with leaving out parts of the program, to learn what error messages you get."

## Objective

Understand what each part of a C program does by deliberately breaking it and
observing which stage of compilation catches each error.

## The Four Experiments

Your task is to create a program that **explains** what happens when you break
a hello world program in four different ways:

| # | Modification | Question |
|---|---|---|
| 1 | Remove `#include <stdio.h>` | What error do you get? Which stage catches it? |
| 2 | Remove the semicolon after `printf()` | What error do you get? Which stage catches it? |
| 3 | Remove `return 0;` | What warning do you get? Which stage catches it? |
| 4 | Misspell `printf` as `prntf` | What error do you get? Which stage catches it? |

## Going Beyond K&R

K&R simply asked you to try these experiments. We go further:

- **Identify the compilation stage** that catches each error (preprocessor,
  parser, semantic analyzer, or linker)
- **Understand why** each stage is responsible for catching that specific error
- **Connect errors to the compilation pipeline** from the chapter lesson

## Building & Testing

```bash
make            # Build starter and solution
make test       # Run tests (solution must pass)
make clean      # Remove built files
```

## What You Should Learn

- The C compilation pipeline has distinct stages, each catching different errors
- `#include` is handled by the preprocessor (stage 1)
- Syntax errors like missing semicolons are caught by the parser (stage 2)
- Missing return values are caught by the semantic analyzer (stage 3)
- Unresolved symbols like misspelled functions are caught by the linker (stage 4)
