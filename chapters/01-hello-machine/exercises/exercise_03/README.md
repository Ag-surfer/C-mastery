# Exercise 03: Process Memory Explorer

**Difficulty:** :star::star::star:

**Original exercise** -- explore your program's memory layout at runtime.

## Objective

Every running C program has its memory organized into distinct segments.
Understanding where your variables actually live in memory is fundamental to
mastering C. This exercise makes the invisible visible by printing the actual
memory addresses of variables in each segment.

## Memory Segments

| Segment | Contains | Lifetime |
|---------|----------|----------|
| **Text/Code** | Machine instructions (your compiled functions) | Entire program |
| **ROData** | String literals, const globals | Entire program |
| **Data** | Initialized global/static variables | Entire program |
| **BSS** | Uninitialized global/static variables (zeroed) | Entire program |
| **Heap** | Dynamically allocated memory (malloc) | Until free() |
| **Stack** | Local variables, function parameters | Until function returns |

## Requirements

Write a program that declares variables in each memory segment and prints
their addresses using `%p`. Your output should show:

1. A **code** address (address of main)
2. A **read-only data** address (a string literal)
3. A **data** address (an initialized global)
4. A **BSS** address (an uninitialized global)
5. A **heap** address (a malloc'd pointer)
6. A **stack** address (a local variable)

## Going Beyond K&R

K&R does not cover process memory layout. This is essential knowledge for:

- Understanding why stack overflows happen
- Understanding why use-after-free is dangerous
- Understanding why buffer overflows in stack variables can overwrite return
  addresses
- Debugging with lldb (examining memory at specific addresses)

## Building & Testing

```bash
make            # Build starter and solution
make test       # Run tests (solution must pass)
make clean      # Remove built files
```

## What You Should Learn

- Every variable has a specific memory address at runtime
- The address reveals which memory segment the variable lives in
- Global variables persist for the entire program (data/BSS segments)
- Local variables are temporary (stack segment)
- malloc'd memory lives in the heap and must be explicitly freed
- Function code itself lives in the text/code segment
