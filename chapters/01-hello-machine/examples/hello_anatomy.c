/*
 * hello_anatomy.c -- Chapter 01: Hello Machine
 * K&R 2nd Ed. Section 1.1 -- "Getting Started"
 * Concept: What really happens when main() runs -- the C runtime,
 *          arguments, return values, and cleanup.
 *
 * Compile: gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *          hello_anatomy.c -o hello_anatomy
 * Run:     ./hello_anatomy arg1 arg2 && echo "Exit status: $?"
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * STARTUP SEQUENCE (what happens before main):
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * When you type ./hello_anatomy, the following occurs:
 *
 *   1. The kernel creates a new process and loads the Mach-O binary.
 *   2. dyld (the dynamic linker) maps shared libraries (libSystem.dylib,
 *      which contains libc) into the process address space.
 *   3. dyld calls library initializers (C++ constructors, ObjC +load, etc.)
 *   4. dyld transfers control to the program entry point.
 *   5. The C runtime startup code (in libSystem) calls main(argc, argv).
 *
 * The key insight: main() is NOT the first thing that runs.
 * There is an entire runtime setup phase before your code executes.
 */

/*
 * cleanup() -- registered with atexit() to run after main returns.
 * This demonstrates the shutdown sequence: after main returns,
 * the C runtime calls atexit-registered functions in reverse order,
 * flushes stdio buffers, then calls _exit().
 */
static void cleanup(void) {
  printf("\n[atexit] cleanup() called -- program is shutting down.\n");
  printf("[atexit] stdio buffers will be flushed after this.\n");
  printf("[atexit] Then _exit() syscall releases all process memory.\n");
}

/*
 * int main(int argc, char *argv[])
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * CONCEPT:   This is the full signature of main (K&R 1.1 shows the
 *            simpler main() form; 5.10 introduces argc/argv).
 *            - argc: argument count (always >= 1, since argv[0] is the
 *                    program name)
 *            - argv: array of C strings, one per argument
 *            - argv[argc] is guaranteed to be NULL (C11 5.1.2.2.1)
 *
 * ASSEMBLY:  On ARM64, the calling convention passes:
 *            - argc in register w0 (32-bit int)
 *            - argv in register x1 (64-bit pointer)
 *
 *   _main:
 *     sub  sp, sp, #96              ; allocate 96-byte stack frame
 *     stp  x29, x30, [sp, #80]     ; save frame pointer + link register
 *     add  x29, sp, #80            ; establish frame pointer at sp+80
 *     stur wzr, [x29, #-4]         ; implicit return value slot = 0
 *     stur w0, [x29, #-8]          ; spill argc to stack
 *     stur x1, [x29, #-16]         ; spill argv to stack
 *
 * MEMORY:    main's stack frame layout (actual -O0 output):
 *     [x29 - 4]   : return value slot (4 bytes, int)
 *     [x29 - 8]   : argc (4 bytes, int)
 *     [x29 - 16]  : argv (8 bytes, pointer on 64-bit)
 *     [x29 - 20]  : loop variable i (4 bytes, int)
 *     [sp+80]     : saved x29 (frame pointer) + x30 (link register)
 *     The frame pointer (x29) = sp + 80.
 *     The link register (x30) holds the return address.
 */
int main(int argc, char *argv[]) {
  printf("=== Hello Machine: Anatomy of main() ===\n\n");

  /*
   * Register the cleanup function with atexit().
   * atexit() returns 0 on success, non-zero on failure.
   * Functions are called in LIFO (reverse) order on program exit.
   */
  if (atexit(cleanup) != 0) {
    fprintf(stderr, "Failed to register atexit handler\n");
    return 1;
  }

  /* --- argc and argv exploration --- */

  // MEMORY: argc is an int (4 bytes) stored on the stack.
  //         argv is a pointer (8 bytes on 64-bit) to an array of
  //         char pointers, each pointing to a NUL-terminated string.
  printf("--- argc / argv ---\n");
  printf("argc        = %d\n", argc);
  printf("sizeof(argc)= %zu bytes (int)\n", sizeof(argc));
  // Note: argv is declared as char *argv[] but in a function parameter
  // list, arrays decay to pointers.  So sizeof(argv) == sizeof(char **).
  printf("sizeof(argv)= %zu bytes (char** pointer)\n", sizeof(char **));
  printf("argv addr   = %p\n", (const void *)argv);

  printf("\nArguments:\n");
  for (int i = 0; i < argc; i++) {
    // MEMORY: Each argv[i] is a char* pointer (8 bytes).
    //         The strings themselves are stored in a block of memory
    //         set up by the OS before main() is called, typically
    //         near the top of the stack.
    printf("  argv[%d] = \"%s\" (at %p, string at %p)\n", i, argv[i],
           (const void *)&argv[i], (const void *)argv[i]);
  }
  // C11 guarantees argv[argc] == NULL
  printf("  argv[%d] = %p (NULL sentinel)\n", argc, (const void *)argv[argc]);

  /* --- Stack frame exploration --- */

  printf("\n--- Stack Frame ---\n");
  printf("Address of argc:  %p\n", (const void *)&argc);
  printf("Address of argv:  %p\n", (const void *)&argv);
  printf("Address of main:  %p  (in __TEXT,__text)\n", (const void *)main);

  // MEMORY: The stack grows downward on ARM64.
  //         Local variables have higher addresses than deeper call frames.
  //         argc and argv are the first locals on main's stack frame.

  /* --- Return value --- */

  printf("\n--- Return Value ---\n");
  printf("main() will return 0 (success).\n");
  printf("After return, the shell stores it in $?.\n");
  printf("Verify with: ./hello_anatomy ; echo \"Exit status: $?\"\n");

  // ASSEMBLY: stur wzr, [x29, #-4]         ; store 0 in return value slot
  //           ldur w0, [x29, #-4]         ; reload return value into w0
  //           ldp  x29, x30, [sp, #80]    ; restore FP and LR
  //           add  sp, sp, #96            ; deallocate stack frame
  //           ret                         ; return to C runtime
  //
  // After main returns, the C runtime:
  //   1. Calls atexit handlers (our cleanup function)
  //   2. Flushes stdio buffers (stdout, stderr)
  //   3. Calls _exit(0) syscall to terminate the process
  return 0;
}
