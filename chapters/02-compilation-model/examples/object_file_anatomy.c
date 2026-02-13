/*
 * object_file_anatomy.c -- Chapter 02: Compilation Model
 * K&R 2nd Ed. Section 4.1 (Basics of Functions), Section 4.6 (Static)
 * Concept: What the compiler produces -- the .o (object) file contains
 *          machine code, symbol tables, relocation entries, and section
 *          metadata.  Every variable and function maps to a specific
 *          Mach-O section.  This file creates symbols of every type
 *          so you can inspect them with nm and otool.
 *
 * Compile:   gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g \
 *            object_file_anatomy.c -o object_file_anatomy
 * Object:    gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g -c \
 *            object_file_anatomy.c -o object_file_anatomy.o
 * Symbols:   nm object_file_anatomy.o
 * Sections:  otool -l object_file_anatomy.o | grep -A5 sectname
 * Sanitize:  gcc -fsanitize=address,undefined -g \
 *            object_file_anatomy.c -o object_file_anatomy
 *
 * EXPECTED nm OUTPUT (Apple Clang 17, ARM64 Mach-O):
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   (column 1: address, column 2: type, column 3: symbol name)
 *
 *   ________________ S _banner                    ; global, const ptr
 * (__DATA,__const)
 *   ________________ D _global_initialized        ; global, initialized data
 *   ________________ C _global_uninitialized      ; common (tentative
 * definition)
 *   ________________ T _main                      ; global, text (code)
 *   ________________ d _main.static_var           ; local, initialized data (*)
 *                    U _printf                    ; undefined, external
 *   ________________ t _static_helper             ; local, text (code)
 *
 * nm SYMBOL TYPES:
 *   T = global text (code), visible to linker from other .o files
 *   t = local text (code), NOT visible to linker (static function)
 *   D = global initialized data (__DATA,__data)
 *   d = local initialized data (static variables)
 *   C = common symbol (tentative definition, allocated at link time)
 *   S = global symbol in special section (const data, etc.)
 *   s = local zero-fill or other special section (string literals, etc.)
 *   U = undefined (external reference, resolved by linker)
 *
 * (*) Note: Apple Clang mangles static local variable names by prepending
 *     the function name: static_var in main() becomes _main.static_var.
 *
 * EXPECTED otool -l SECTIONS:
 *   Section __text       in __TEXT   -- executable machine code
 *   Section __cstring    in __TEXT   -- string literals ("Hello from...")
 *   Section __data       in __DATA   -- initialized global/static data
 *   Section __const      in __DATA   -- const pointers (banner)
 *   Section __compact_unwind in __LD -- unwind info for exceptions
 *   (common symbols are tentative -- no section in .o until link time)
 */

#include <stdint.h>
#include <stdio.h>

/* -----------------------------------------------------------------------
 * GLOBAL INITIALIZED VARIABLE
 * nm type: D (global data)
 * Mach-O section: __DATA,__data
 * // MEMORY: Stored in the __DATA,__data section of the Mach-O binary.
 * //         The value 42 is written into the binary at compile time.
 * //         sizeof(int) = 4 bytes.
 * ----------------------------------------------------------------------- */
int global_initialized = 42;

/* -----------------------------------------------------------------------
 * GLOBAL UNINITIALIZED VARIABLE
 * nm type: C (common -- tentative definition)
 * Mach-O: allocated by the linker in __DATA,__common (then __bss at load)
 * // MEMORY: NOT stored in the binary (saves disk space).  The linker
 * //         allocates space in __DATA,__common.  The loader zero-fills
 * //         this memory at program startup.
 * //         sizeof(int) = 4 bytes, initialized to 0 by C11 guarantee.
 * ----------------------------------------------------------------------- */
int global_uninitialized;

/* -----------------------------------------------------------------------
 * CONST POINTER TO STRING LITERAL
 * banner is a const pointer (const char *const) stored as a global.
 * The pointer itself: nm type S (global, special section / __DATA,__const)
 * The string it points to: in __TEXT,__cstring (read-only)
 * // MEMORY: The pointer (8 bytes) is in __DATA,__const.  The string
 * //         "=== Object File Anatomy ===" (28 bytes) is in __TEXT,__cstring.
 * //         Writing to the string triggers EXC_BAD_ACCESS on macOS.
 * ----------------------------------------------------------------------- */
const char *const banner = "=== Object File Anatomy ===";

/* -----------------------------------------------------------------------
 * STATIC FUNCTION (file-local)
 * nm type: t (local text -- lowercase means NOT visible to linker)
 * Mach-O section: __TEXT,__text
 * // ASSEMBLY: The function body is in __TEXT,__text just like main(),
 * //           but the linker will NOT export this symbol.  Other .o files
 * //           cannot call static_helper().
 * //           _static_helper:
 * //             sub   sp, sp, #16        ; 16-byte stack frame (leaf)
 * //             str   w0, [sp, #12]      ; spill argument a
 * //             ldr   w8, [sp, #12]      ; load a
 * //             ldr   w9, [sp, #12]      ; load a again
 * //             mul   w0, w8, w9         ; a * a -> return in w0
 * //             add   sp, sp, #16
 * //             ret
 * // MEMORY: Machine code in __TEXT,__text.  Same segment as main().
 * ----------------------------------------------------------------------- */
static int static_helper(int a) { return a * a; }

/*
 * int main(void)
 * nm type: T (global text -- visible to linker and dynamic loader)
 * Mach-O section: __TEXT,__text
 *
 * // ASSEMBLY: The compiler generates ARM64 machine code for main().
 * //           External calls (printf) generate:
 * //             bl  _printf
 * //           This is a RELOCATION: the linker must patch the bl target
 * //           address because printf's address is unknown at compile time.
 * //           In the .o file, the bl offset is 0 -- a placeholder.
 * //           The linker fills in the real address when building the binary.
 *
 * // MEMORY: main() code lives in __TEXT,__text (read-only, executable).
 * //         printf's address is resolved at link time (static linking)
 * //         or at load time via dyld (dynamic linking with libSystem).
 */
int main(void) {
  printf("%s\n\n", banner);

  /* --- Static local variable ---
   * nm type: d (local data -- appears as _main.static_var in nm)
   * Mach-O section: __DATA,__data (initialized static)
   * K&R 4.6: A static variable inside a function retains its value
   * between calls.  It is initialized once (at program load) and persists
   * for the lifetime of the process.
   *
   * // MEMORY: static_var is NOT on the stack.  Despite being declared
   * //         inside main(), it lives in __DATA,__data because it must
   * //         persist across calls.  sizeof(int) = 4 bytes.
   */
  static int static_var = 100;

  printf("--- Symbol Locations (runtime addresses) ---\n");
  printf("  global_initialized  : %p  (__DATA,__data)\n",
         (const void *)&global_initialized);
  printf("  global_uninitialized: %p  (__DATA,__common -> __bss)\n",
         (const void *)&global_uninitialized);
  printf("  static_var          : %p  (__DATA,__data, file-local)\n",
         (const void *)&static_var);
  printf("  banner (ptr)        : %p  (pointer in __DATA)\n",
         (const void *)&banner);
  printf("  banner (string)     : %p  (__TEXT,__cstring)\n",
         (const void *)banner);
  printf("  main()              : %p  (__TEXT,__text)\n", (const void *)main);
  printf("  static_helper()     : %p  (__TEXT,__text, file-local)\n",
         (const void *)static_helper);
  printf("  printf()            : %p  (libSystem, resolved by dyld)\n",
         (const void *)printf);

  printf("\n--- Symbol Sizes ---\n");
  printf("  sizeof(global_initialized)   = %zu bytes\n",
         sizeof(global_initialized));
  printf("  sizeof(global_uninitialized)  = %zu bytes\n",
         sizeof(global_uninitialized));
  printf("  sizeof(static_var)           = %zu bytes\n", sizeof(static_var));
  printf("  sizeof(banner)               = %zu bytes (pointer, not string)\n",
         sizeof(banner));
  printf("  sizeof(\"=== Object...\")      = %zu bytes (string + NUL)\n",
         sizeof("=== Object File Anatomy ==="));

  printf("\n--- Symbol Values ---\n");
  printf("  global_initialized  = %d\n", global_initialized);
  printf("  global_uninitialized= %d  (C11 guarantees 0 for file-scope)\n",
         global_uninitialized);
  printf("  static_var          = %d\n", static_var);
  printf("  static_helper(7)    = %d  (7*7, called but linker-invisible)\n",
         static_helper(7));

  printf("\n--- Section Summary ---\n");
  printf("  __TEXT,__text    : main(), static_helper() -- executable code\n");
  printf("  __TEXT,__cstring : string literals -- read-only data\n");
  printf("  __DATA,__data   : global_initialized, static_var -- read/write\n");
  printf("  __DATA,__common : global_uninitialized -- zero-filled at load\n");
  printf("  U (undefined)   : printf -- resolved by linker from libSystem\n");

  printf("\n--- Inspect This File ---\n");
  printf("  gcc -c -O0 -g object_file_anatomy.c -o object_file_anatomy.o\n");
  printf("  nm object_file_anatomy.o           # symbol table\n");
  printf("  otool -l object_file_anatomy.o     # section listing\n");
  printf("  strings object_file_anatomy.o      # embedded strings\n");
  printf("  size object_file_anatomy.o         # segment sizes\n");

  return 0;
}
