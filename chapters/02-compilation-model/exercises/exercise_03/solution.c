/*
 * solution.c — Separate Compilation
 * Chapter 02: Compilation Model
 * Exercise 03 (Original)
 *
 * K&R Reference: Section 4.5 (Header Files), Section 4.6 (Static Variables)
 *
 * Reference solution: main() includes the shared header and calls
 * functions defined in solution_helper.c. The linker resolves the
 * cross-file references.
 */
#include <stdio.h>
#include <stdlib.h>

#include "solution_helper.h"

int main(void) {
  printf("factorial(5) = %d\n", factorial(5));
  printf("factorial(10) = %d\n", factorial(10));
  printf("fibonacci(10) = %d\n", fibonacci(10));
  printf("fibonacci(20) = %d\n", fibonacci(20));
  printf("Total function calls: %d\n", call_count);

  return EXIT_SUCCESS;
}
