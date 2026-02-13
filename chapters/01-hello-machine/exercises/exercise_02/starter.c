/*
 * starter.c
 * Chapter 01 — Hello Machine
 * Exercise 02 — Escape Sequence Lab
 * Adapted from K&R Exercise 1-2
 *
 * CONCEPT: Every escape sequence is a single byte with a specific numeric
 *          value. This exercise builds a table showing the connection
 *          between escape notation and byte values.
 *
 * INSTRUCTIONS: Implement print_escape_table() to print a formatted table
 * of escape sequences with their decimal and hexadecimal values.
 *
 * Required escapes: \n, \t, \\, \", \0, \a, \b, \r
 */
#include <stdio.h>

/*
 * TODO: Print a formatted table of escape sequences.
 *
 * Each row should show:
 *   - The escape notation (e.g., \n)
 *   - The decimal value (e.g., 10)
 *   - The hex value (e.g., 0x0a)
 *   - A description (e.g., Newline)
 *
 * Hint: Use printf with %d and %#04x format specifiers.
 * Hint: For \0, print its integer value — don't try to print it as a char.
 */
void print_escape_table(void) {
  printf("TODO: Implement escape sequence table\n");
}

int main(void) {
  printf("=== Escape Sequence Lab ===\n");
  printf("Adapted from K&R Exercise 1-2\n\n");

  print_escape_table();

  return 0;
}
