/*
 * test.c
 * Chapter 01 — Hello Machine
 * Exercise 03 — Process Memory Explorer
 * Original exercise
 *
 * Test harness: runs ./solution and verifies that all 6 memory segment
 * labels are present and each has a hex address (0x...).
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

/* Check that a line containing 'label' also contains '0x' (a hex address) */
static int label_has_hex_address(const char *output, const char *label) {
  const char *pos = strstr(output, label);
  if (!pos) {
    return 0;
  }
  /* Search for 0x within the next 80 characters (same line) */
  const char *end = pos + 80;
  const char *output_end = output + strlen(output);
  if (end > output_end) {
    end = output_end;
  }
  for (const char *p = pos; p < end; p++) {
    if (*p == '\n') {
      break;
    }
    if (p[0] == '0' && p + 1 < end && p[1] == 'x') {
      return 1;
    }
  }
  return 0;
}

int main(void) {
  printf("Testing Exercise 03: Process Memory Explorer\n");
  printf("=============================================\n\n");

  char *output = capture_output();
  ASSERT(output != NULL);
  if (!output) {
    printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
    return EXIT_FAILURE;
  }

  /* Test that all 6 segment labels are present */
  printf("Test: All segment labels present...\n");
  ASSERT(contains(output, "Code"));
  ASSERT(contains(output, "ROData") || contains(output, "rodata") ||
         contains(output, "string"));
  ASSERT(contains(output, "Data"));
  ASSERT(contains(output, "BSS"));
  ASSERT(contains(output, "Heap"));
  ASSERT(contains(output, "Stack"));

  /* Test that each label line has a hex address (0x...) */
  printf("Test: Each segment shows a hex address...\n");
  ASSERT(label_has_hex_address(output, "Code"));
  ASSERT(label_has_hex_address(output, "Data"));
  ASSERT(label_has_hex_address(output, "BSS"));
  ASSERT(label_has_hex_address(output, "Heap"));
  ASSERT(label_has_hex_address(output, "Stack"));

  /* Test that the output mentions memory layout */
  printf("Test: Output is about memory layout...\n");
  ASSERT(contains(output, "Memory Layout"));

  printf("\nResults: %d passed, %d failed\n", tests_passed, tests_failed);

  free(output);
  return tests_failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
