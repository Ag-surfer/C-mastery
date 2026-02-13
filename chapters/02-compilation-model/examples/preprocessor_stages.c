/*
 * preprocessor_stages.c -- Chapter 02: Compilation Model
 * K&R 2nd Ed. Section 4.11 (The C Preprocessor), Appendix A12
 * Concept: The preprocessor is a text-substitution engine that runs
 *          BEFORE the compiler ever sees your code.  Macros, conditionals,
 *          and file inclusion are resolved entirely at compile time --
 *          they have zero runtime cost and leave zero footprint in the binary.
 *
 * Compile:   gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *            preprocessor_stages.c -o preprocessor_stages
 * Preprocess: gcc -E preprocessor_stages.c | wc -l
 *            (expect ~700-900 lines after #include <stdio.h> expansion)
 * Sanitize:  gcc -fsanitize=address,undefined -g \
 *            preprocessor_stages.c -o preprocessor_stages
 */

#include <stdint.h>
#include <stdio.h>

/* ========================================================================
 * STAGE 1: Object-like macros (K&R 4.11.1)
 * ========================================================================
 * Object-like macros are simple text replacements.  The preprocessor
 * replaces every occurrence of the macro name with its definition.
 *
 * // ASSEMBLY: After preprocessing, MAX_BUFFER appears nowhere in the
 * //           assembly output.  The compiler sees only the literal 1024.
 * //           mov x8, #1024          ; the macro is gone (x8 for vararg)
 *
 * // MEMORY: Macros are NOT variables.  They occupy no memory -- no
 * //         stack slot, no data segment entry, no symbol table entry.
 * //         They are pure text substitution, erased before compilation.
 */
#define MAX_BUFFER 1024
#define PI 3.14159265358979
#define VERSION_STR "2.0.1"

/* ========================================================================
 * STAGE 2: Function-like macros (K&R 4.11.2)
 * ========================================================================
 * Function-like macros look like function calls but expand inline.
 * They have zero call overhead (no bl instruction, no stack frame).
 *
 * WARNING: Macro arguments are expanded by text substitution, so
 * SQUARE(x+1) becomes (x+1)*(x+1) only if you parenthesize properly.
 * Without parens, SQUARE(x+1) would become x+1*x+1 = x + x + 1 (wrong).
 *
 * // ASSEMBLY: SQUARE(5) becomes (5)*(5).  The compiler sees:
 * //           mov w8, #25            ; constant folded at compile time
 * //           No function call, no stack setup, no bl instruction.
 *
 * // MEMORY: Zero overhead.  The expansion is inlined at every call site.
 * //         This can increase code size if the macro body is large.
 */
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b, type)                                                       \
  do {                                                                         \
    type _tmp = (a);                                                           \
    (a) = (b);                                                                 \
    (b) = _tmp;                                                                \
  } while (0)

/* ========================================================================
 * STAGE 3: Stringification (#) and Token Pasting (##)
 * ========================================================================
 * K&R A12.3: The # operator converts a macro argument to a string literal.
 * K&R A12.3: The ## operator concatenates two tokens into one.
 *
 * These are powerful metaprogramming tools for generating code and
 * debug output at compile time.
 *
 * // ASSEMBLY: STRINGIFY(hello) becomes "hello" -- a string literal
 * //           stored in __TEXT,__cstring.  No runtime computation.
 * // MEMORY: The resulting string occupies read-only memory in
 * __TEXT,__cstring.
 */
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define PRINT_VAR(var) printf("  " #var " = %d\n", (var))

/* ========================================================================
 * STAGE 4: Conditional compilation (K&R 4.11.3)
 * ========================================================================
 * #ifdef, #ifndef, #if, #elif, #else, #endif
 *
 * The preprocessor includes or excludes entire blocks of code before
 * the compiler sees them.  Excluded code generates zero assembly,
 * zero object code, and zero binary footprint.
 *
 * // ASSEMBLY: Code inside a false #ifdef is completely absent from
 * //           the assembly output.  It was never compiled.
 * // MEMORY: Excluded code takes zero bytes in the final binary.
 */
#ifndef PLATFORM_NAME
#define PLATFORM_NAME "default"
#endif

#ifdef DEBUG
#define DEBUG_PRINT(msg) printf("[DEBUG] %s:%d: %s\n", __FILE__, __LINE__, msg)
#else
#define DEBUG_PRINT(msg) ((void)0) /* expands to nothing in release builds */
#endif

/* ========================================================================
 * STAGE 5: Predefined macros (K&R A12.8, C11 6.10.8)
 * ========================================================================
 * The compiler provides several built-in macros.  These are resolved
 * at preprocessing time -- their values are baked into the source
 * before compilation begins.
 *
 * __FILE__          - current source filename (string literal)
 * __LINE__          - current line number (integer constant)
 * __DATE__          - compilation date "Mmm dd yyyy" (string literal)
 * __TIME__          - compilation time "hh:mm:ss" (string literal)
 * __STDC__          - 1 if conforming implementation (always 1)
 * __STDC_VERSION__  - C standard version: 201112L for C11
 *
 * // MEMORY: __FILE__ and __DATE__ become string literals in __TEXT,__cstring.
 * //         __LINE__ becomes an integer constant (immediate operand).
 */

/* Helper: print a separator line */
static void print_section(const char *title) {
  printf("\n=== %s ===\n", title);
}

int main(void) {
  printf("Chapter 02: Preprocessor Stages\n");
  printf("================================\n");

  /* --- Stage 1: Object-like macros --- */
  print_section("Stage 1: Object-Like Macros");

  // MEMORY: MAX_BUFFER is NOT a variable.  The compiler sees the literal
  //         1024 directly.  sizeof(MAX_BUFFER) is sizeof(int) = 4 bytes,
  //         but that 4 bytes is the literal, not a stored variable.
  printf("  MAX_BUFFER   = %d\n", MAX_BUFFER);
  printf("  PI           = %.15f\n", PI);
  printf("  VERSION_STR  = \"%s\"\n", VERSION_STR);
  printf("  sizeof(int)  = %zu  (MAX_BUFFER is just the literal 1024)\n",
         sizeof(int));

  /* --- Stage 2: Function-like macros --- */
  print_section("Stage 2: Function-Like Macros");

  int val = 7;
  // ASSEMBLY: SQUARE(val) expands to ((val) * (val)).
  //           ldur w9, [x29, #-8]          ; load val
  //           ldur w10, [x29, #-8]         ; load val again
  //           mul  w10, w9, w10            ; multiply
  //           No bl instruction -- zero call overhead.
  printf("  SQUARE(%d)      = %d\n", val, SQUARE(val));
  printf("  SQUARE(3+1)     = %d  (correct due to parentheses)\n",
         SQUARE(3 + 1));

  int a = 10, b = 20;
  printf("  MAX(%d, %d)     = %d\n", a, b, MAX(a, b));

  // ASSEMBLY: SWAP expands to a do { ... } while(0) block.
  //           Three stores and three loads -- no function call.
  printf("  Before SWAP: a=%d, b=%d\n", a, b);
  SWAP(a, b, int);
  printf("  After  SWAP: a=%d, b=%d\n", a, b);

  /* --- Stage 3: Stringification and Token Pasting --- */
  print_section("Stage 3: Stringification (#) and Token Pasting (##)");

  // ASSEMBLY: STRINGIFY(hello_world) becomes "hello_world" at preprocessing.
  //           adrp x0, l_.str.N@PAGE       ; load string literal address
  //           The string is in __TEXT,__cstring -- read-only, no runtime cost.
  printf("  STRINGIFY(hello_world) = \"%s\"\n", STRINGIFY(hello_world));
  printf("  STRINGIFY(3 + 4)       = \"%s\"\n", STRINGIFY(3 + 4));

  // Token pasting: CONCAT(my_, var) becomes my_var at preprocessing.
  int CONCAT(my_, var) = 42;
  printf("  CONCAT(my_, var) created variable my_var = %d\n", my_var);

  // PRINT_VAR uses # to stringify the variable name for debug output.
  int counter = 99;
  PRINT_VAR(counter);

  /* --- Stage 4: Conditional compilation --- */
  print_section("Stage 4: Conditional Compilation");

  printf("  PLATFORM_NAME = \"%s\"\n", PLATFORM_NAME);

#ifdef DEBUG
  printf("  DEBUG is defined -- debug code is compiled\n");
#else
  printf("  DEBUG is NOT defined -- debug code excluded from binary\n");
#endif

  // MEMORY: The DEBUG_PRINT call below expands to ((void)0) when DEBUG
  //         is not defined.  This generates zero code -- not even a nop.
  //         The string argument is never stored in the binary.
  DEBUG_PRINT("This message only appears in debug builds");

  printf("  Compile with -DDEBUG to enable debug output\n");

  /* --- Stage 5: Predefined macros --- */
  print_section("Stage 5: Predefined Macros");

  // MEMORY: __FILE__ resolves to a string literal stored in __TEXT,__cstring.
  //         __LINE__ resolves to an integer constant (immediate operand).
  printf("  __FILE__          = \"%s\"\n", __FILE__);
  printf("  __LINE__          = %d\n", __LINE__);
  printf("  __DATE__          = \"%s\"\n", __DATE__);
  printf("  __TIME__          = \"%s\"\n", __TIME__);
  printf("  __STDC__          = %d  (1 = conforming implementation)\n",
         __STDC__);
  printf("  __STDC_VERSION__  = %ldL", __STDC_VERSION__);
  if (__STDC_VERSION__ == 201112L) {
    printf("  (C11)\n");
  } else if (__STDC_VERSION__ == 201710L) {
    printf("  (C17)\n");
  } else {
    printf("\n");
  }

  /* --- Key Takeaway --- */
  print_section("Key Takeaway");
  printf("  The preprocessor is a TEXT SUBSTITUTION engine.\n");
  printf("  It runs BEFORE compilation.  Macros are resolved, conditionals\n");
  printf("  select code paths, and #include pastes file contents.\n");
  printf("  After preprocessing, the compiler sees only pure C -- no # "
         "directives.\n");
  printf("  Use 'gcc -E preprocessor_stages.c' to see the preprocessor "
         "output.\n");

  return 0;
}
