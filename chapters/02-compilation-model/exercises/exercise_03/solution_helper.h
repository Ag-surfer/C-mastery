/*
 * solution_helper.h — Separate Compilation
 * Chapter 02: Compilation Model
 * Exercise 03 (Original)
 *
 * Header file: declares the shared interface between translation units.
 */
#ifndef SOLUTION_HELPER_H
#define SOLUTION_HELPER_H

/* Declare (not define) the global call counter.
 * "extern" tells the compiler: this variable exists somewhere else. */
extern int call_count;

/* Function prototypes — the compiler needs these to type-check calls
 * in other translation units. */
int factorial(int n);
int fibonacci(int n);

#endif /* SOLUTION_HELPER_H */
