/*
 * starter.c — Preprocessor Detective
 * Chapter 02: Compilation Model
 * Exercise 01 (Original)
 *
 * K&R Reference: Section 4.11 (The C Preprocessor)
 *
 * Instructions:
 *   Fill in each printf with the value you PREDICT the preprocessor
 *   will produce. Then compile and run to verify your predictions.
 *   Use "gcc -E starter.c" to see the actual preprocessed output.
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
  /* TODO: Replace 0 with your predicted value for MAX_SIZE */
  printf("MAX_SIZE = %d\n", 0);

  /* TODO: Replace 0 with your predicted value for SQUARE(5) */
  printf("SQUARE(5) = %d\n", 0);

  /*
   * Token pasting: CONCAT(my, _var) pastes "my" and "_var" into one token.
   * TODO: What variable name does this create? What is its value?
   *       Replace the 0 below with the correct variable name.
   */
  int CONCAT(my, _var) = 99;
  (void)my_var; /* suppress unused warning for now */
  printf("CONCAT(my,_var) = %d\n", 0);

  /* TODO: Replace "???" with what STRINGIFY(hello) produces */
  printf("STRINGIFY(hello) = %s\n", "???");

  /* TODO: Replace "???" with this file's name (predefined macro) */
  printf("__FILE__ = %s\n", "???");

  /* TODO: Replace 0 with the current line number (predefined macro) */
  printf("__LINE__ = %d\n", 0);

  /* TODO: Replace 0L with the C standard version (predefined macro) */
  printf("__STDC_VERSION__ = %ld\n", 0L);

  /* Conditional compilation: this block only compiles with -DDEBUG */
#ifdef DEBUG
  printf("DEBUG block: compiled\n");
#else
  printf("DEBUG block: not compiled\n");
#endif

  return EXIT_SUCCESS;
}
