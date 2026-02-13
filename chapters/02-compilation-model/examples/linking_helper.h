/*
 * linking_helper.h -- Chapter 02: Compilation Model
 * K&R 2nd Ed. Section 4.5 (Header Files), Section 4.11 (#include)
 * Concept: Header files declare interfaces -- function prototypes and
 *          extern variable declarations -- so that separate .c files
 *          can reference each other's symbols without knowing the
 *          implementation details.
 *
 * The #ifndef / #define / #endif pattern is an "include guard" that
 * prevents multiple-inclusion errors when the same header is #included
 * from multiple source files or transitively through other headers.
 *
 * // MEMORY: This file generates ZERO machine code and ZERO data.
 * //         It contains only declarations, not definitions.
 * //         The compiler uses it for type checking but discards it
 * //         after preprocessing.  nm will show no symbols from this file.
 */

#ifndef LINKING_HELPER_H
#define LINKING_HELPER_H

/*
 * Function declaration (prototype).
 * Tells the compiler: "add() exists, takes two ints, returns an int."
 * The DEFINITION (actual code) is in linking_helper.c.
 *
 * Without this declaration, calling add() from linking_main.c would
 * produce an "implicit function declaration" warning (error with -Werror).
 */
int add(int a, int b);

/*
 * Extern variable declaration.
 * Tells the compiler: "shared_counter exists as an int somewhere."
 * The DEFINITION (with initial value) is in linking_helper.c.
 *
 * 'extern' means "this variable is defined in another translation unit."
 * It creates a symbol reference (U in nm) that the linker must resolve.
 */
extern int shared_counter;

/*
 * Function declaration for increment_counter.
 * Modifies shared_counter -- demonstrates cross-file variable access.
 */
void increment_counter(void);

#endif /* LINKING_HELPER_H */
