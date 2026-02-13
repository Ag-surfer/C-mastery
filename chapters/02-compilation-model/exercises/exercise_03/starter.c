/*
 * starter.c — Separate Compilation
 * Chapter 02: Compilation Model
 * Exercise 03 (Original)
 *
 * K&R Reference: Section 4.5 (Header Files), Section 4.6 (Static Variables)
 *
 * This is a monolithic single-file program. Your task is to split it
 * into three files:
 *
 *   solution.c          — main() function
 *   solution_helper.h   — header with declarations (extern, prototypes)
 *   solution_helper.c   — function and variable definitions
 *
 * The split program must produce IDENTICAL output to this monolithic version.
 *
 * TODO:
 *   1. Create solution_helper.h with include guards, extern declaration
 *      for call_count, and function prototypes.
 *   2. Create solution_helper.c with #include "solution_helper.h" and
 *      the definitions of call_count, factorial(), and fibonacci().
 *   3. Create solution.c with #include "solution_helper.h" and main().
 *   4. Build: gcc solution.c solution_helper.c -o solution
 */
#include <stdio.h>
#include <stdlib.h>

/* Global counter — incremented by each function call */
int call_count = 0;

/* Compute n! (n factorial) iteratively */
int factorial(int n) {
  call_count++;
  int result = 1;
  for (int i = 2; i <= n; i++) {
    result *= i;
  }
  return result;
}

/* Compute the nth Fibonacci number iteratively */
int fibonacci(int n) {
  call_count++;
  if (n <= 0)
    return 0;
  if (n == 1)
    return 1;
  int prev = 0;
  int curr = 1;
  for (int i = 2; i <= n; i++) {
    int next = prev + curr;
    prev = curr;
    curr = next;
  }
  return curr;
}

int main(void) {
  printf("factorial(5) = %d\n", factorial(5));
  printf("factorial(10) = %d\n", factorial(10));
  printf("fibonacci(10) = %d\n", fibonacci(10));
  printf("fibonacci(20) = %d\n", fibonacci(20));
  printf("Total function calls: %d\n", call_count);

  return EXIT_SUCCESS;
}
