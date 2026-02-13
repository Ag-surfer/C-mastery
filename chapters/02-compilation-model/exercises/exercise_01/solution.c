/*
 * solution.c — Preprocessor Detective
 * Chapter 02: Compilation Model
 * Exercise 01 (Original)
 *
 * K&R Reference: Section 4.11 (The C Preprocessor)
 *
 * Reference solution: each printf uses the correct macro expansion.
 */
#include <stdio.h>
#include <stdlib.h>

/* --- Object-like macro --- */
#define MAX_SIZE 100

/* --- Function-like macro --- */
#define SQUARE(x) ((x) * (x))

/* --- Token pasting --- */
#define CONCAT(a, b) a##b

/* --- Stringification --- */
#define STRINGIFY(x) #x

int main(void) {
  /* Object-like macro: MAX_SIZE expands to the integer literal 100 */
  printf("MAX_SIZE = %d\n", MAX_SIZE);

  /* Function-like macro: SQUARE(5) expands to ((5) * (5)) = 25 */
  printf("SQUARE(5) = %d\n", SQUARE(5));

  /* Token pasting: CONCAT(my, _var) creates the identifier my_var */
  int CONCAT(my, _var) = 99;
  printf("CONCAT(my,_var) = %d\n", my_var);

  /* Stringification: STRINGIFY(hello) expands to "hello" */
  printf("STRINGIFY(hello) = %s\n", STRINGIFY(hello));

  /* Predefined macros provided by the compiler */
  printf("__FILE__ = %s\n", __FILE__);
  printf("__LINE__ = %d\n", __LINE__);
  printf("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);

  /* Conditional compilation: without -DDEBUG, the #else branch runs */
#ifdef DEBUG
  printf("DEBUG block: compiled\n");
#else
  printf("DEBUG block: not compiled\n");
#endif

  return EXIT_SUCCESS;
}
