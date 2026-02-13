/*
 * test.c
 * Chapter 01 — Hello Machine
 * Exercise 02 — Escape Sequence Lab
 * Adapted from K&R Exercise 1-2
 *
 * Test harness: runs ./solution and verifies that the escape sequence
 * table contains correct decimal and hex values for each escape.
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

static char *capture_output(void) {
  FILE *fp = popen("./solution", "r");
  if (!fp) {
    fprintf(stderr, "ERROR: Could not run ./solution\n");
    return NULL;
  }

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
  pclose(fp);

  return buffer;
}

static int contains(const char *haystack, const char *needle) {
  if (!haystack || !needle) {
    return 0;
  }
  return strstr(haystack, needle) != NULL;
}

int main(void) {
  printf("Testing Exercise 02: Escape Sequence Lab\n");
  printf("=========================================\n\n");

  char *output = capture_output();
  ASSERT(output != NULL);
  if (!output) {
    printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
    return EXIT_FAILURE;
  }

  /* Test \\n: decimal 10, hex 0x0a */
  printf("Test: \\n = 10 = 0x0a...\n");
  ASSERT(contains(output, "\\n"));
  ASSERT(contains(output, "10"));
  ASSERT(contains(output, "0x0a"));

  /* Test \\t: decimal 9, hex 0x09 */
  printf("Test: \\t = 9 = 0x09...\n");
  ASSERT(contains(output, "\\t"));
  ASSERT(contains(output, "9"));
  ASSERT(contains(output, "0x09"));

  /* Test \\\\: decimal 92, hex 0x5c */
  printf("Test: \\\\ = 92 = 0x5c...\n");
  ASSERT(contains(output, "\\\\"));
  ASSERT(contains(output, "92"));
  ASSERT(contains(output, "0x5c"));

  /* Test \\\": decimal 34, hex 0x22 */
  printf("Test: \\\" = 34 = 0x22...\n");
  ASSERT(contains(output, "\\\""));
  ASSERT(contains(output, "34"));
  ASSERT(contains(output, "0x22"));

  /* Test \\0: decimal 0, hex 0x00 */
  printf("Test: \\0 = 0 = 0x00...\n");
  ASSERT(contains(output, "\\0"));
  ASSERT(contains(output, "0x00"));

  /* Test \\a: decimal 7, hex 0x07 */
  printf("Test: \\a = 7 = 0x07...\n");
  ASSERT(contains(output, "\\a"));
  ASSERT(contains(output, "0x07"));

  /* Test \\b: decimal 8, hex 0x08 */
  printf("Test: \\b = 8 = 0x08...\n");
  ASSERT(contains(output, "\\b"));
  ASSERT(contains(output, "0x08"));

  /* Test \\r: decimal 13, hex 0x0d */
  printf("Test: \\r = 13 = 0x0d...\n");
  ASSERT(contains(output, "\\r"));
  ASSERT(contains(output, "13"));
  ASSERT(contains(output, "0x0d"));

  printf("\nResults: %d passed, %d failed\n", tests_passed, tests_failed);

  free(output);
  return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
