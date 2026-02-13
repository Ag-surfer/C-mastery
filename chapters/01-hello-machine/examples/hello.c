/*
 * hello.c -- Chapter 01: Hello Machine
 * K&R 2nd Ed. Section 1.1 -- "Getting Started"
 * Concept: The simplest C program, annotated to show every stage
 *          from source code to running process.
 *
 * Compile: gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g hello.c -o hello
 * Run:     ./hello && echo "Exit status: $?"
 */

/*
 * PREPROCESSOR STAGE:
 * #include <stdio.h> is a preprocessor directive.  Before compilation
 * even begins, the preprocessor replaces this line with the entire
 * contents of stdio.h (~700 lines on macOS).  This gives us the
 * declaration of printf().  Without it, the compiler would not know
 * printf's signature and would emit an error under -Werror.
 *
 * Verify yourself:  gcc -E hello.c | wc -l
 * (You will see several hundred lines of expanded header.)
 */
#include <stdio.h>

/*
 * int main(void)
 * ~~~~~~~~~~~~~~
 * CONCEPT:   main() is the entry point that the C runtime calls after
 *            the OS loads the program and the dynamic linker (dyld on
 *            macOS) resolves shared libraries.
 *
 * SIGNATURE: int main(void) tells the compiler:
 *            - Returns an int (the exit status, 0 = success)
 *            - Takes no arguments (void)
 *
 * ASSEMBLY:  The linker marks _main as the entry symbol.
 *            On ARM64 macOS, the call chain is:
 *            dyld -> libSystem_initializer -> _main
 *
 * MEMORY:    main's machine code lives in the __TEXT,__text section
 *            of the Mach-O binary.  This section is mapped read-only
 *            and executable into the process's virtual address space.
 */
int main(void) {
  /*
   * printf("Hello, World!\n")
   * ~~~~~~~~~~~~~~~~~~~~~~~~~
   * CONCEPT:   printf is a variadic function declared in <stdio.h>.
   *            The format string "Hello, World!\n" contains no format
   *            specifiers, so printf simply writes the literal bytes
   *            to stdout (file descriptor 1).
   *
   * K&R 1.1:   "The only way to learn a new programming language is
   *            by writing programs in it.  The first program to write
   *            is the same for all languages: print the words
   *            hello, world."
   */

  // MEMORY: "Hello, World!\n" is a string literal stored in the
  //         __TEXT,__cstring section of the Mach-O binary.  It occupies
  //         15 bytes (14 chars + NUL terminator).  This section is
  //         read-only; attempting to modify it is undefined behavior.
  // ASSEMBLY: adrp x0, l_.str@PAGE       ; load page address of string
  //           add  x0, x0, l_.str@PAGEOFF ; add offset within page
  //           bl   _printf               ; branch-with-link to printf
  printf("Hello, World!\n");

  /*
   * Verify our claims about the string literal at runtime.
   * sizeof on a string literal includes the NUL terminator.
   */
  printf("\n--- Verification ---\n");
  printf("sizeof(\"Hello, World!\\n\") = %zu bytes\n",
         sizeof("Hello, World!\n"));
  printf("String literal address:    %p\n", (const void *)"Hello, World!\n");
  printf("main() address:            %p\n", (const void *)main);

  /*
   * return 0
   * ~~~~~~~~
   * CONCEPT:   Returning 0 from main tells the OS the program succeeded.
   *            The shell stores this in $? (echo $? to see it).
   *            Non-zero values signal an error.
   *
   * ASSEMBLY:  ldur w0, [x29, #-8]       ; reload return value from stack spill
   *            ldp  x29, x30, [sp, #32] ; restore frame pointer & link register
   *            add  sp, sp, #48         ; deallocate stack frame
   *            ret                      ; return to caller (_exit path)
   *
   * MEMORY:    After return, the C runtime calls atexit handlers,
   *            flushes stdio buffers, then issues the _exit syscall.
   *            The kernel reclaims all process memory.
   */
  return 0;
}
