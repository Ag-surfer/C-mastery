/*
 * test.c
 * Chapter 01 — Hello Machine
 * Exercise 01 — Break Hello World
 * Adapted from K&R Exercise 1-1
 *
 * Test harness: runs ./solution and checks that its output contains
 * the required educational content about compilation errors.
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

/* Read all output from running ./solution into a buffer */
static char *capture_output(void) {
  FILE *fp = popen("./solution", "r");
  if (!fp) {
    fprintf(stderr, "ERROR: Could not run ./solution\n");
    return NULL;
  }

  /* Allocate buffer for output */
  size_t capacity = 4096;
  size_t length = 0;
  char *buffer = malloc(capacity);
  if (!buffer) {
    pclose(fp);
    return NULL;
  }

  size_t n;
  while ((n = fread(buffer + length, 1, capacity - length - 1, fp)) > 0) {
    length += n;
    if (length + 1 >= capacity) {
      capacity *= 2;
      char *new_buf = realloc(buffer, capacity);
      if (!new_buf) {
        free(buffer);
        pclose(fp);
        return NULL;
      }
      buffer = new_buf;
    }
  }
  buffer[length] = '\0';

  int status = pclose(fp);
  if (status != 0) {
    fprintf(stderr, "WARNING: ./solution exited with status %d\n", status);
  }

  return buffer;
}

/* Case-insensitive substring search */
static int contains(const char *haystack, const char *needle) {
  if (!haystack || !needle) {
    return 0;
  }
  /* Simple case-insensitive search */
  size_t hay_len = strlen(haystack);
  size_t need_len = strlen(needle);
  if (need_len > hay_len) {
    return 0;
  }
  for (size_t i = 0; i <= hay_len - need_len; i++) {
    size_t j;
    for (j = 0; j < need_len; j++) {
      char h = haystack[i + j];
      char n = needle[j];
      /* Lowercase both */
      if (h >= 'A' && h <= 'Z') {
        h = (char)(h + ('a' - 'A'));
      }
      if (n >= 'A' && n <= 'Z') {
        n = (char)(n + ('a' - 'A'));
      }
      if (h != n) {
        break;
      }
    }
    if (j == need_len) {
      return 1;
    }
  }
  return 0;
}

int main(void) {
  printf("Testing Exercise 01: Break Hello World\n");
  printf("=======================================\n\n");

  char *output = capture_output();
  ASSERT(output != NULL);
  if (!output) {
    printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
    return EXIT_FAILURE;
  }

  /* Test Experiment 1: Missing #include should mention implicit/undeclared */
  printf("Test 1: Experiment 1 mentions implicit or undeclared...\n");
  ASSERT(contains(output, "implicit") || contains(output, "undeclared"));

  /* Test Experiment 2: Missing semicolon should mention syntax or expected */
  printf("Test 2: Experiment 2 mentions syntax or expected...\n");
  ASSERT(contains(output, "syntax") || contains(output, "expected"));

  /* Test Experiment 3: Missing return should mention return */
  printf("Test 3: Experiment 3 mentions return...\n");
  ASSERT(contains(output, "return"));

  /* Test Experiment 4: Misspelled function should mention undefined or linker
   */
  printf("Test 4: Experiment 4 mentions undefined or linker...\n");
  ASSERT(contains(output, "undefined") || contains(output, "linker"));

  /* Test that all 4 experiments are covered */
  printf("Test 5: All 4 experiments present...\n");
  ASSERT(contains(output, "experiment 1"));
  ASSERT(contains(output, "experiment 2"));
  ASSERT(contains(output, "experiment 3"));
  ASSERT(contains(output, "experiment 4"));

  /* Test that compilation stages are mentioned */
  printf("Test 6: Compilation stages mentioned...\n");
  ASSERT(contains(output, "parser") || contains(output, "syntax"));
  ASSERT(contains(output, "linker"));
  ASSERT(contains(output, "semantic") || contains(output, "analyzer"));

  printf("\nResults: %d passed, %d failed\n", tests_passed, tests_failed);

  free(output);
  return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
