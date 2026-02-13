/*
 * solution.c
 * Chapter 01 — Hello Machine
 * Exercise 02 — Escape Sequence Lab
 * Adapted from K&R Exercise 1-2
 *
 * CONCEPT: Every escape sequence is a single byte with a specific numeric
 *          value. This table shows the connection between escape notation,
 *          the character it produces, and its underlying byte value.
 */
#include <stdio.h>

void print_escape_table(void) {
  printf("Escape  Decimal  Hex     Description\n");
  printf("------  -------  ------  ----------------\n");

  /* \n = newline = 10 = 0x0a */
  printf("\\n      %-7d  0x%02x    Newline\n", '\n', (unsigned char)'\n');

  /* \t = horizontal tab = 9 = 0x09 */
  printf("\\t      %-7d  0x%02x    Horizontal tab\n", '\t',
         (unsigned char)'\t');

  /* \\ = backslash = 92 = 0x5c */
  printf("\\\\      %-7d  0x%02x    Backslash\n", '\\', (unsigned char)'\\');

  /* \" = double quote = 34 = 0x22 */
  printf("\\\"      %-7d  0x%02x    Double quote\n", '\"', (unsigned char)'\"');

  /* \0 = null terminator = 0 = 0x00 */
  printf("\\0      %-7d  0x%02x    Null terminator\n", '\0',
         (unsigned char)'\0');

  /* \a = alert/bell = 7 = 0x07 */
  printf("\\a      %-7d  0x%02x    Alert (bell)\n", '\a', (unsigned char)'\a');

  /* \b = backspace = 8 = 0x08 */
  printf("\\b      %-7d  0x%02x    Backspace\n", '\b', (unsigned char)'\b');

  /* \r = carriage return = 13 = 0x0d */
  printf("\\r      %-7d  0x%02x    Carriage return\n", '\r',
         (unsigned char)'\r');
}

int main(void) {
  printf("=== Escape Sequence Lab ===\n");
  printf("Adapted from K&R Exercise 1-2\n\n");

  print_escape_table();

  return 0;
}
