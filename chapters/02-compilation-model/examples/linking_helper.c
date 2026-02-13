/*
 * linking_helper.c -- Chapter 02: Compilation Model
 * K&R 2nd Ed. Section 4.1 (Basics of Functions), Section 4.6 (Static)
 * Concept: Separate compilation -- this file is compiled independently
 *          into linking_helper.o, then linked with linking_main.o to
 *          form the final executable.  Demonstrates how the linker
 *          resolves symbol references across translation units.
 *
 * Compile to object: gcc -Wall -Wextra -Werror -std=c11 -pedantic \
 *                    -O0 -g -c linking_helper.c -o linking_helper.o
 * Inspect symbols:   nm linking_helper.o
 *
 * EXPECTED nm OUTPUT for linking_helper.o:
 *   ________________ T _add                ; global text (visible to linker)
 *   ________________ T _increment_counter  ; global text (visible to linker)
 *   ________________ S _shared_counter     ; global, zero-fill section
 *   ________________ t _square_internal    ; local text (NOT visible)
 *
 * Note: shared_counter = 0 is placed in __DATA,__common (zero-fill) by
 * the compiler because its initial value is zero.  nm shows 'S' not 'D'.
 * After linking, the linker merges it into __DATA,__bss or __DATA,__common.
 *
 * // MEMORY: This translation unit contributes to Mach-O sections:
 * //   __TEXT,__text    : add(), increment_counter(), square_internal()
 * //   __DATA,__common  : shared_counter (4 bytes, zero-initialized)
 * //   (no __TEXT,__cstring -- this file has no string literals)
 */

#include "linking_helper.h"

/* -----------------------------------------------------------------------
 * GLOBAL VARIABLE DEFINITION
 * This is the DEFINITION that matches the 'extern' DECLARATION in the header.
 * nm type: S (global, zero-fill -- because initial value is 0)
 * Mach-O section: __DATA,__common (zero-filled at load)
 *
 * // MEMORY: shared_counter occupies 4 bytes.  Since it is initialized to 0,
 * //         Apple Clang places it in __DATA,__common (zero-fill section)
 * //         rather than __DATA,__data.  Both .o files reference the same
 * //         4 bytes -- the linker ensures exactly one copy exists.
 * ----------------------------------------------------------------------- */
int shared_counter = 0;

/* -----------------------------------------------------------------------
 * STATIC (FILE-LOCAL) FUNCTION
 * nm type: t (local text -- lowercase = NOT visible to linker)
 * Only callable from within this file (linking_helper.c).
 * Other .o files cannot call square_internal() even if they try.
 *
 * K&R 4.6: "static applied to a function means the function is
 *           visible only within the source file in which it is defined."
 *
 * // ASSEMBLY: square_internal appears in __TEXT,__text of linking_helper.o,
 * //           but with local linkage.  The linker ignores it when resolving
 * //           symbols from other .o files.
 * //           _square_internal:
 * //             sub   sp, sp, #16        ; 16-byte stack frame (leaf)
 * //             str   w0, [sp, #12]      ; spill argument a
 * //             ldr   w8, [sp, #12]      ; load a
 * //             ldr   w9, [sp, #12]      ; load a again
 * //             mul   w0, w8, w9         ; a * a -> return in w0
 * //             add   sp, sp, #16
 * //             ret
 *
 * // MEMORY: Code in __TEXT,__text.  Same segment as add() and
 * //         increment_counter(), but invisible to the linker.
 * ----------------------------------------------------------------------- */
static int square_internal(int a) { return a * a; }

/* -----------------------------------------------------------------------
 * GLOBAL FUNCTION DEFINITION
 * nm type: T (global text -- visible to linker)
 * This is the DEFINITION that matches the DECLARATION in the header.
 *
 * // ASSEMBLY: In linking_main.o, the call to add() generates:
 * //             bl  _add
 * //           The bl offset is 0 in the .o file (a relocation entry).
 * //           The linker patches it to the actual address of _add
 * //           from linking_helper.o when producing the final binary.
 *
 * // MEMORY: Code in __TEXT,__text of linking_helper.o.
 * //         After linking, it occupies the same __TEXT segment as main().
 * ----------------------------------------------------------------------- */
int add(int a, int b) { return a + b; }

/* -----------------------------------------------------------------------
 * INCREMENT_COUNTER
 * Modifies the global shared_counter variable.
 * Demonstrates cross-file variable access: linking_main.c reads
 * shared_counter, and this function writes to it.
 *
 * // ASSEMBLY:
 * //   mov    w0, #1                       ; arg for square_internal(1)
 * //   bl     _square_internal             ; returns 1*1 = 1 in w0
 * //   adrp   x9, _shared_counter@PAGE    ; load page of shared_counter
 * //   ldr    w8, [x9, _shared_counter@PAGEOFF] ; load current value
 * //   add    w8, w8, w0                   ; add return value
 * //   str    w8, [x9, _shared_counter@PAGEOFF] ; store back
 * //
 * // MEMORY: Reads and writes _shared_counter in __DATA,__common.
 * //         This is the SAME memory that linking_main.c reads via
 * //         the extern declaration.
 * ----------------------------------------------------------------------- */
void increment_counter(void) {
  /* Use square_internal to demonstrate the static helper is callable here */
  shared_counter += square_internal(1); /* adds 1*1 = 1 each call */
}
