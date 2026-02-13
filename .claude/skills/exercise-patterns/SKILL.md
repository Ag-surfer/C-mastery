---
name: exercise-patterns
description: Exercise structure, test harness patterns, and difficulty progression. Auto-loads when editing exercise files.
globs: ["**/exercises/**"]
---

## Exercise Directory Structure
exercise_NN/
├── README.md      # Problem statement
├── starter.c      # Compiles but fails tests (student fills in)
├── solution.c     # Reference solution
├── test.c         # Auto-grading test harness
└── Makefile       # build, test, clean, asm targets

## README Template
````markdown
# Exercise NN-M: Title
**Difficulty:** ★☆☆ / ★★☆ / ★★★
**K&R Reference:** Exercise X-Y (page Z)
**Concepts:** comma-separated list

## Problem
Clear problem statement. What to implement.

## Requirements
- Specific function signatures
- Edge cases to handle
- Constraints

## Hints
- Assembly: what instruction should your solution produce?
- Memory: where should your data live?

## Testing
```bash
make test    # Run test harness
make asm     # View your assembly output
```
````

## Difficulty Progression
- ★☆☆ Beginner: Direct application of chapter concept. K&R exercises 1-1 through 1-10 level.
- ★★☆ Intermediate: Combines multiple concepts. K&R exercises requiring helper functions.
- ★★★ Advanced: Requires understanding of machine behavior, edge cases, optimization.

## Test Harness Pattern
````c
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Include the student's implementation
#include "starter.c"  // or solution.c for validation

int tests_passed = 0;
int tests_failed = 0;

#define TEST(name, expr) do { \
    if (expr) { tests_passed++; printf("  PASS: %s\n", name); } \
    else { tests_failed++; printf("  FAIL: %s (line %d)\n", name, __LINE__); } \
} while(0)

int main(void) {
    printf("Running tests...\n\n");
    // TEST("description", condition);
    printf("\nResults: %d passed, %d failed\n", tests_passed, tests_failed);
    return tests_failed > 0 ? 1 : 0;
}
````
