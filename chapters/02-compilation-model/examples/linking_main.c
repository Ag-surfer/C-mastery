/*
 * linking_main.c -- Chapter 02: Compilation Model
 * K&R 2nd Ed. Section 4.1 (Basics of Functions), Section 4.5 (Header Files)
 * Concept: Linker symbol resolution -- how separate .o files are combined
 *          into a single executable.  This file references symbols defined
 *          in linking_helper.c (add, shared_counter, increment_counter).
 *          The linker resolves these references at link time.
 *
 * === BUILD STEPS (the compilation pipeline in action) ===
 *
 * Step 1 -- Preprocess + Compile each file independently:
 *   gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *       -c linking_main.c -o linking_main.o
 *   gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *       -c linking_helper.c -o linking_helper.o
 *
 * Step 2 -- Link the object files together:
 *   gcc linking_main.o linking_helper.o -o linking_demo
 *
 * Step 3 -- Run:
 *   ./linking_demo
 *
 * Alternatively, compile and link in one step:
 *   gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *       linking_main.c linking_helper.c -o linking_demo
 *
 * EXPECTED nm OUTPUT for linking_main.o:
 *   ________________ T _main              ; global text (defined here)
 *                    U _add               ; undefined (defined in helper.o)
 *                    U _increment_counter ; undefined (defined in helper.o)
 *                    U _printf            ; undefined (defined in libSystem)
 *                    U _shared_counter    ; undefined (defined in helper.o)
 *
 * KEY INSIGHT: In linking_main.o, add/increment_counter/shared_counter
 * are all "U" (undefined).  The linker finds their definitions in
 * linking_helper.o and patches the references.  printf is found in
 * libSystem.dylib by the dynamic linker (dyld).
 *
 * // ASSEMBLY: bl _add in linking_main.o has a placeholder offset.
 * //           The linker patches it to the real address of _add from
 * //           linking_helper.o.  This is called RELOCATION.
 * //           After linking, both .o files' __TEXT sections are merged
 * //           into one contiguous __TEXT segment in the final Mach-O binary.
 *
 * // MEMORY: After linking:
 * //   __TEXT,__text  : main() + add() + increment_counter() +
 * square_internal()
 * //   __TEXT,__cstring: all string literals from both files
 * //   __DATA,__data : shared_counter (one copy, shared)
 * //   The two .o files' sections are concatenated and their symbol
 * //   references resolved to final addresses.
 */

#include "linking_helper.h"
#include <stdio.h>

int main(void) {
  printf("Chapter 02: Linker Symbol Resolution Demo\n");
  printf("==========================================\n\n");

  /* --- Cross-file function call ---
   * // ASSEMBLY: bl _add
   * //           In linking_main.o, this is an unresolved reference.
   * //           The linker finds _add (type T) in linking_helper.o
   * //           and patches the bl instruction with the correct offset.
   */
  int result = add(17, 25);
  printf("--- Cross-File Function Call ---\n");
  printf("  add(17, 25) = %d\n", result);
  printf("  add() is defined in linking_helper.c\n");
  printf("  In linking_main.o, _add is 'U' (undefined)\n");
  printf("  The linker resolves it from linking_helper.o\n");

  /* --- Cross-file variable access ---
   * // ASSEMBLY: adrp x8, _shared_counter@GOTPAGE
   * //           ldr  x8, [x8, _shared_counter@GOTPAGEOFF]
   * //           Because shared_counter is extern, the compiler uses
   * //           GOT-indirect addressing (@GOTPAGE/@GOTPAGEOFF) instead
   * //           of direct @PAGE/@PAGEOFF.  The linker fills in the GOT
   * //           entry with shared_counter's final address.
   */
  printf("\n--- Cross-File Variable Access ---\n");
  printf("  shared_counter (before) = %d\n", shared_counter);
  printf("  shared_counter addr     = %p\n", (const void *)&shared_counter);

  /* Call increment_counter 3 times */
  increment_counter();
  increment_counter();
  increment_counter();

  printf("  shared_counter (after 3 increments) = %d\n", shared_counter);
  printf("  increment_counter() modifies shared_counter in linking_helper.c\n");
  printf("  Both files access the SAME 4 bytes in __DATA,__data\n");

  /* --- Symbol addresses demonstrate segment layout --- */
  printf("\n--- Symbol Addresses (Segment Layout) ---\n");
  printf("  main()              = %p  (__TEXT,__text)\n", (const void *)main);
  printf("  add()               = %p  (__TEXT,__text)\n", (const void *)add);
  printf("  increment_counter() = %p  (__TEXT,__text)\n",
         (const void *)increment_counter);
  printf("  &shared_counter     = %p  (__DATA,__data)\n",
         (const void *)&shared_counter);
  printf("  printf()            = %p  (libSystem via dyld)\n",
         (const void *)printf);

  /* --- Size verification --- */
  printf("\n--- Type Sizes ---\n");
  printf("  sizeof(shared_counter) = %zu bytes (int)\n",
         sizeof(shared_counter));
  printf("  sizeof(result)         = %zu bytes (int)\n", sizeof(result));
  printf("  sizeof(void *)         = %zu bytes (pointer on this platform)\n",
         sizeof(void *));

  /* --- Build steps reminder --- */
  printf("\n--- Try It Yourself ---\n");
  printf("  # Compile separately:\n");
  printf("  gcc -c linking_main.c -o linking_main.o\n");
  printf("  gcc -c linking_helper.c -o linking_helper.o\n");
  printf("  # Inspect unresolved symbols:\n");
  printf("  nm linking_main.o | grep ' U '    # shows _add, "
         "_shared_counter, _printf\n");
  printf("  nm linking_helper.o | grep ' T '  # shows _add, "
         "_increment_counter\n");
  printf("  # Link together:\n");
  printf("  gcc linking_main.o linking_helper.o -o linking_demo\n");
  printf("  # Inspect resolved symbols:\n");
  printf("  nm linking_demo                   # no more 'U' "
         "(except libSystem)\n");

  return 0;
}
