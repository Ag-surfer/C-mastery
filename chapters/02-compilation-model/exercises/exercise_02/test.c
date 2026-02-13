/*
 * test.c — Object File Explorer
 * Chapter 02: Compilation Model
 * Exercise 02 — Test Harness
 *
 * Runs ./solution and verifies its output contains the correct
 * nm symbol type letters and descriptions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT(expr)                                                           \
  do {                                                                         \
    if (expr) {                                                                \
      tests_passed++;                                                          \
    } else {                                                                   \
      tests_failed++;                                                          \
      fprintf(stderr, "FAIL: %s (line %d)\n", #expr, __LINE__);                \
    }                                                                          \
  } while (0)

#define TEST(name) static void test_##name(void)

#define OUTPUT_BUF_SIZE 4096

static char output[OUTPUT_BUF_SIZE];

static int capture_solution_output(void) {
  FILE *fp = popen("./solution", "r");
  if (!fp) {
    fprintf(stderr, "ERROR: could not run ./solution\n");
    return -1;
  }

  size_t total = 0;
  while (total < OUTPUT_BUF_SIZE - 1) {
    size_t n = fread(output + total, 1, OUTPUT_BUF_SIZE - 1 - total, fp);
    if (n == 0)
      break;
    total += n;
  }
  output[total] = '\0';

  int status = pclose(fp);
  return status;
}

TEST(global_function_symbol) {
  /* global_function should be T (global text) */
  ASSERT(strstr(output, "global_function:    T") != NULL);
  ASSERT(strstr(output, "global text") != NULL);
}

TEST(local_function_symbol) {
  /* local_function (static) should be t (local text) */
  ASSERT(strstr(output, "local_function:     t") != NULL);
  ASSERT(strstr(output, "local text") != NULL);
}

TEST(printf_symbol) {
  /* printf should be U (undefined, external) */
  ASSERT(strstr(output, "printf:             U") != NULL);
  ASSERT(strstr(output, "undefined") != NULL);
}

TEST(global_initialized_symbol) {
  /* global_initialized should be D (global data) */
  ASSERT(strstr(output, "global_initialized: D") != NULL);
  ASSERT(strstr(output, "global data") != NULL);
}

TEST(global_uninitialized_symbol) {
  /* global_uninitialized should be C (common) */
  ASSERT(strstr(output, "global_uninitialized: C") != NULL);
  ASSERT(strstr(output, "common") != NULL);
}

TEST(static_var_symbol) {
  /* static_var should be d (local data) */
  ASSERT(strstr(output, "static_var:         d") != NULL);
  ASSERT(strstr(output, "local data") != NULL);
}

TEST(values_correct) {
  /* Verify the actual variable values are printed */
  ASSERT(strstr(output, "global_initialized = 42") != NULL);
  ASSERT(strstr(output, "global_uninitialized = 0") != NULL);
  ASSERT(strstr(output, "static_var = 7") != NULL);
}

int main(void) {
  int status = capture_solution_output();
  if (status != 0) {
    fprintf(stderr, "ERROR: ./solution exited with status %d\n", status);
    fprintf(stderr, "Build the solution first: make solution\n");
    return EXIT_FAILURE;
  }

  test_global_function_symbol();
  test_local_function_symbol();
  test_printf_symbol();
  test_global_initialized_symbol();
  test_global_uninitialized_symbol();
  test_static_var_symbol();
  test_values_correct();

  printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
  return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
