/*
 * starter.c — Object File Explorer
 * Chapter 02: Compilation Model
 * Exercise 02 (Original — requires nm analysis)
 *
 * Instructions:
 *   This file declares several entities with different storage classes.
 *   Each declaration produces a different symbol type in the object file.
 *
 *   1. Replace each "?" with the correct nm symbol type letter.
 *   2. Replace each "???" description with the correct meaning.
 *   3. Compile and verify: gcc -c starter.c -o starter.o && nm -p starter.o
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
   * TODO: Replace '?' with the correct nm symbol type letter.
   *       Replace "???" with the correct description.
   *
   * Hint: uppercase = global, lowercase = local
   * Hint: T=text(code), D=data(init), C=common(uninit), U=undefined
   */
  printf("global_function:    %c (%s)\n", '?', "???");
  printf("local_function:     %c (%s)\n", '?', "???");
  printf("printf:             %c (%s)\n", '?', "???");
  printf("global_initialized: %c (%s)\n", '?', "???");
  printf("global_uninitialized: %c (%s)\n", '?', "???");
  printf("static_var:         %c (%s)\n", '?', "???");

  return EXIT_SUCCESS;
}
