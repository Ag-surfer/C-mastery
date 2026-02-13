/*
 * solution.c
 * Chapter 01 — Hello Machine
 * Exercise 01 — Break Hello World
 * Adapted from K&R Exercise 1-1
 *
 * CONCEPT: Each part of a C program is checked by a different stage of
 *          the compilation pipeline. By deliberately breaking things,
 *          we learn which stage is responsible for what.
 */
#include <stdio.h>

static void print_experiment_1(void) {
  printf("Experiment 1: Remove #include <stdio.h>\n");
  printf("  Error: implicit declaration of function 'printf'\n");
  printf("  Stage: Semantic Analyzer (compile phase)\n");
  printf("  Why:   Without the header, the compiler has no declaration\n");
  printf("         for printf(). In C11 with -Werror, calling an\n");
  printf("         undeclared function is an error. The preprocessor\n");
  printf("         stage handles #include, but the missing declaration\n");
  printf("         is caught during semantic analysis.\n");
}

static void print_experiment_2(void) {
  printf("Experiment 2: Remove semicolon after printf()\n");
  printf("  Error: expected ';' after expression\n");
  printf("  Stage: Parser (syntax analysis)\n");
  printf("  Why:   The parser checks that the source code follows C\n");
  printf("         grammar rules. Every statement must end with a\n");
  printf("         semicolon. The parser detects the missing ';'\n");
  printf("         when it encounters the next token unexpectedly.\n");
}

static void print_experiment_3(void) {
  printf("Experiment 3: Remove return 0;\n");
  printf("  Warning/Error: non-void function does not return a value\n");
  printf("  Stage: Semantic Analyzer (with -Werror makes it an error)\n");
  printf("  Why:   main() is declared as returning int. The semantic\n");
  printf("         analyzer checks that all code paths return a value.\n");
  printf("         Note: C11 actually allows omitting return from\n");
  printf("         main() specifically (it defaults to 0), but -Wall\n");
  printf("         -Werror flags it as an error for safety.\n");
}

static void print_experiment_4(void) {
  printf("Experiment 4: Misspell printf as prntf\n");
  printf("  Error: undefined symbol '_prntf' (linker error)\n");
  printf("  Stage: Linker\n");
  printf("  Why:   With #include <stdio.h> present, the compiler sees\n");
  printf("         no declaration for 'prntf' and flags an implicit\n");
  printf("         declaration error. Without the header, the compiler\n");
  printf("         may accept it, but the linker fails because no\n");
  printf("         library provides a function named 'prntf'.\n");
  printf("         The linker resolves symbol references to actual\n");
  printf("         addresses — if a symbol doesn't exist, it fails.\n");
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
