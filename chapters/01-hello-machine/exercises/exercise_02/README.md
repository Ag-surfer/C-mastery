# Exercise 02: Escape Sequence Lab

**Difficulty:** :star::star:

**Adapted from K&R Exercise 1-2:** "Experiment to find out what happens when
printf's argument string contains \\c, where c is some character not listed
above."

## Objective

Understand C escape sequences at the byte level. Every escape sequence maps to
a specific numeric value (a single byte). You will build a formatted table that
shows the escape notation, the character it produces, and its numeric value in
both decimal and hexadecimal.

## Requirements

Implement the function `print_escape_table(void)` that prints a formatted table
of the following escape sequences:

| Escape | Decimal | Hex  | Description      |
|--------|---------|------|------------------|
| `\n`   | 10      | 0x0a | Newline          |
| `\t`   | 9       | 0x09 | Horizontal tab   |
| `\\`   | 92      | 0x5c | Backslash        |
| `\"`   | 34      | 0x22 | Double quote     |
| `\0`   | 0       | 0x00 | Null terminator  |
| `\a`   | 7       | 0x07 | Alert (bell)     |
| `\b`   | 8       | 0x08 | Backspace        |
| `\r`   | 13      | 0x0d | Carriage return  |

## Going Beyond K&R

- K&R lists escape sequences as syntax. We treat them as **byte values**.
- Each escape sequence is a single byte with a specific numeric value.
- Understanding this connection between notation and numeric value is essential
  for working with binary data, network protocols, and file formats.

## Building & Testing

```bash
make            # Build starter and solution
make test       # Run tests (solution must pass)
make clean      # Remove built files
```

## What You Should Learn

- Every character in C is a number (its ASCII/byte value)
- Escape sequences are syntactic sugar for specific byte values
- `\0` is the number zero, not the character '0' (which is 48 / 0x30)
- Printf format specifiers let you view the same byte as a character, decimal,
  or hex value
