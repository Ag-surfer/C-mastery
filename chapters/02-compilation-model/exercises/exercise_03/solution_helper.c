/*
 * solution_helper.c — Separate Compilation
 * Chapter 02: Compilation Model
 * Exercise 03 (Original)
 *
 * Definitions of helper functions and the shared global variable.
 * This file is compiled separately and linked with solution.c.
 */
#include "solution_helper.h"

/* Define the global call counter (storage allocated here) */
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
