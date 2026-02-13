/*
 * solution.c — Object File Explorer
 * Chapter 02: Compilation Model
 * Exercise 02 (Original — requires nm analysis)
 *
 * Reference solution: each printf uses the correct nm symbol type
 * as observed on macOS (Mach-O object files, Apple Clang).
 *
 * To verify: gcc -c solution.c -o solution.o && nm -p solution.o
 */
#include <stdio.h>
#include <stdlib.h>

/* A global (non-static) function — visible to the linker */
void global_function(void) { printf("I am a global function\n"); }

/* A static (file-local) function — NOT visible to the linker */
static void local_function(void) { printf("I am a local function\n"); }

/* A global initialized variable */
int global_initialized = 42;

/* A global uninitialized variable (tentative definition) */
int global_uninitialized;

/* A static (file-local) initialized variable */
static int static_var = 7;

int main(void) {
  /* Call both functions to prevent unused warnings */
  global_function();
  local_function();

  /* Use all variables to prevent unused warnings */
  printf("global_initialized = %d\n", global_initialized);
  printf("global_uninitialized = %d\n", global_uninitialized);
  printf("static_var = %d\n", static_var);

  printf("\n--- Symbol Table Predictions ---\n");

  /*
   * nm symbol types on macOS (Mach-O):
   *   T = global text (code)     t = local text (code)
   *   D = global data (init)     d = local data (init)
   *   C = common (uninit global) U = undefined (external)
   */
  printf("global_function:    %c (%s)\n", 'T', "global text/code");
  printf("local_function:     %c (%s)\n", 't', "local text/code");
  printf("printf:             %c (%s)\n", 'U',
         "undefined, resolved at link time");
  printf("global_initialized: %c (%s)\n", 'D', "global data, initialized");
  printf("global_uninitialized: %c (%s)\n", 'C',
         "common, uninitialized global");
  printf("static_var:         %c (%s)\n", 'd', "local data, initialized");

  return EXIT_SUCCESS;
}
