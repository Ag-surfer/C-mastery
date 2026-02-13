/*
 * starter.c
 * Chapter 01 — Hello Machine
 * Exercise 01 — Break Hello World
 * Adapted from K&R Exercise 1-1
 *
 * CONCEPT: Understand what each part of a C program does by learning
 *          which compilation stage catches each type of error.
 *
 * INSTRUCTIONS: Complete the four print_experiment_N() functions below.
 * Each function should print:
 *   - What modification was made
 *   - What error or warning results
 *   - Which compilation stage catches it (preprocessor, parser,
 *     semantic analyzer, or linker)
 */
#include <stdio.h>

/*
 * TODO 1: Explain what happens when you remove #include <stdio.h>.
 * Try it yourself first! Then fill in this function with the explanation.
 * Hint: Which compilation stage resolves #include directives?
 */
static void print_experiment_1(void) {
  printf("Experiment 1: Remove #include <stdio.h>\n");
  printf("TODO: Describe the error and which stage catches it\n");
}

/*
 * TODO 2: Explain what happens when you remove the semicolon after printf().
 * Hint: Which stage checks for correct grammar/syntax?
 */
static void print_experiment_2(void) {
  printf("Experiment 2: Remove semicolon after printf()\n");
  printf("TODO: Describe the error and which stage catches it\n");
}

/*
 * TODO 3: Explain what happens when you remove "return 0;" from main().
 * Hint: This is a warning, not an error (in C11). Which stage checks types
 * and return values?
 */
static void print_experiment_3(void) {
  printf("Experiment 3: Remove return 0;\n");
  printf("TODO: Describe the error and which stage catches it\n");
}

/*
 * TODO 4: Explain what happens when you misspell "printf" as "prntf".
 * Hint: The compiler compiles successfully, but something fails when
 * combining object files with libraries. Which stage is that?
 */
static void print_experiment_4(void) {
  printf("Experiment 4: Misspell printf as prntf\n");
  printf("TODO: Describe the error and which stage catches it\n");
}

int main(void) {
  printf("=== Break Hello World ===\n");
  printf("Understanding compilation errors by breaking things\n\n");

  print_experiment_1();
  printf("\n");
  print_experiment_2();
  printf("\n");
  print_experiment_3();
  printf("\n");
  print_experiment_4();
  printf("\n");

  return 0;
}
