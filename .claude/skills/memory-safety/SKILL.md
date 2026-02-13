---
name: memory-safety
description: Memory safety verification patterns. Auto-loads when editing code with malloc/free/pointers.
globs: ["**/*malloc*", "**/*alloc*", "**/*pointer*", "**/*memory*"]
---

## Required Checks for All Dynamic Memory Code
1. Every malloc has a NULL check immediately after
2. Every malloc has a matching free (annotated with lifecycle comment)
3. Freed pointers are set to NULL immediately after free
4. Array accesses are bounds-checked where possible
5. String operations use strncpy/snprintf (never strcpy/sprintf)

## Sanitizer Compilation (Apple Clang 17 — native on macOS ARM)
````bash
# AddressSanitizer (heap buffer overflow, use-after-free, double-free, stack overflow, leaks)
gcc -fsanitize=address -g example.c -o example

# UndefinedBehaviorSanitizer (signed overflow, null deref, shift overflow)
gcc -fsanitize=undefined -g example.c -o example

# Both (recommended for exercises)
gcc -fsanitize=address,undefined -g example.c -o example
````

## Debugger (lldb — macOS native, replaces GDB)
````bash
lldb ./example
# Key commands:
#   breakpoint set -n main     (or: b main)
#   run                        (or: r)
#   step                       (or: s) — step into
#   next                       (or: n) — step over
#   frame variable             (or: fr v) — print local variables
#   bt                         — backtrace
#   memory read &x             — examine memory at address of x
#   register read              — show all registers
````

## Valgrind Note
Valgrind does NOT support Apple Silicon (ARM64). The sanitizers above (-fsanitize=address,undefined)
detect the same class of bugs and are built into Clang 17. No additional installation needed.
If you need Valgrind-style heap profiling, use Instruments.app (Xcode) → Allocations or Leaks.

## Common Memory Bugs (with detection on macOS)
| Bug | ASan | UBSan | lldb |
|---|---|---|---|
| Heap buffer overflow | YES | — | YES |
| Stack buffer overflow | YES | — | YES |
| Use after free | YES | — | YES |
| Double free | YES | — | YES |
| Memory leak | YES (LSan) | — | — |
| Uninitialized read | — | — | YES |
| Signed integer overflow | — | YES | — |
| Null pointer dereference | — | YES | YES |

## Malloc/Free Lifecycle Annotation Pattern
````c
// LIFECYCLE: allocated here, freed at end of process_data()
int *data = malloc(n * sizeof(int));
if (data == NULL) {
    perror("malloc failed");
    return EXIT_FAILURE;
}

// ... use data ...

free(data);       // LIFECYCLE: matches malloc at line NN
data = NULL;      // PITFALL: prevent use-after-free
````

## Safe String Patterns
````c
// PITFALL: strcpy does NOT check bounds — use snprintf instead
char buf[64];
snprintf(buf, sizeof(buf), "Hello, %s", name);  // Always safe

// PITFALL: strncpy does NOT null-terminate if src >= n
char dest[32];
strncpy(dest, src, sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';  // Always null-terminate manually
````
