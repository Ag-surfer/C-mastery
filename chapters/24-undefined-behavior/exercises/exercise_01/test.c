/*
 * Chapter 24 — Undefined Behavior
 * Exercise 01 — Test Harness
 */
#include <stdio.h>
#include <stdlib.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT(expr) do { \
    if (expr) { tests_passed++; } \
    else { tests_failed++; \
        fprintf(stderr, "FAIL: %s (line %d)\n", #expr, __LINE__); } \
} while (0)

int main(void) {
    /* TODO: Add test cases */
    ASSERT(1 == 1);

    printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
    return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
