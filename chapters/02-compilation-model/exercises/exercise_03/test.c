/*
 * test.c — Separate Compilation
 * Chapter 02: Compilation Model
 * Exercise 03 — Test Harness
 *
 * Runs ./solution (the multi-file build) and verifies its output
 * matches the expected factorial, fibonacci, and call count values.
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

TEST(factorial_5) {
  /* 5! = 120 */
  ASSERT(strstr(output, "factorial(5) = 120") != NULL);
}

TEST(factorial_10) {
  /* 10! = 3628800 */
  ASSERT(strstr(output, "factorial(10) = 3628800") != NULL);
}

TEST(fibonacci_10) {
  /* fib(10) = 55 */
  ASSERT(strstr(output, "fibonacci(10) = 55") != NULL);
}

TEST(fibonacci_20) {
  /* fib(20) = 6765 */
  ASSERT(strstr(output, "fibonacci(20) = 6765") != NULL);
}

TEST(call_count) {
  /* 4 function calls total: factorial(5), factorial(10), fib(10), fib(20) */
  ASSERT(strstr(output, "Total function calls: 4") != NULL);
}

TEST(output_order) {
  /* Verify output appears in the expected order */
  const char *p1 = strstr(output, "factorial(5)");
  const char *p2 = strstr(output, "factorial(10)");
  const char *p3 = strstr(output, "fibonacci(10)");
  const char *p4 = strstr(output, "fibonacci(20)");
  const char *p5 = strstr(output, "Total function calls");

  ASSERT(p1 != NULL);
  ASSERT(p2 != NULL);
  ASSERT(p3 != NULL);
  ASSERT(p4 != NULL);
  ASSERT(p5 != NULL);

  if (p1 && p2 && p3 && p4 && p5) {
    ASSERT(p1 < p2);
    ASSERT(p2 < p3);
    ASSERT(p3 < p4);
    ASSERT(p4 < p5);
  }
}

int main(void) {
  int status = capture_solution_output();
  if (status != 0) {
    fprintf(stderr, "ERROR: ./solution exited with status %d\n", status);
    fprintf(stderr, "Build the solution first: make solution\n");
    return EXIT_FAILURE;
  }

  test_factorial_5();
  test_factorial_10();
  test_fibonacci_10();
  test_fibonacci_20();
  test_call_count();
  test_output_order();

  printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
  return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
