/*
 * solution.c
 * Chapter 01 — Hello Machine
 * Exercise 03 — Process Memory Explorer
 * Original exercise
 *
 * CONCEPT: Every running C program organizes memory into segments.
 *          This program prints addresses from each segment, making
 *          the invisible memory layout visible.
 *
 * MEMORY: The segments from low to high address (typical layout):
 *   - Text/Code: machine instructions (read-only, executable)
 *   - ROData: string literals, constants (read-only)
 *   - Data: initialized global/static variables (read-write)
 *   - BSS: uninitialized global/static variables, zeroed (read-write)
 *   - Heap: grows upward from brk (dynamic allocation)
 *   - Stack: grows downward from high addresses (local variables)
 */
#include <stdio.h>
#include <stdlib.h>

/* MEMORY: Initialized global -> DATA segment */
int global_initialized = 42;

/* MEMORY: Uninitialized global -> BSS segment (zeroed by OS) */
int global_uninitialized;

/* MEMORY: Pointer itself in DATA, string bytes in TEXT/RODATA */
const char *string_literal = "Hello";

int main(void) {
  /* MEMORY: Local variable -> STACK (high addresses, grows down) */
  int local_var = 10;

  /* MEMORY: malloc returns address in HEAP (grows up from brk) */
  int *heap_var = malloc(sizeof(int));
  if (!heap_var) {
    fprintf(stderr, "malloc failed\n");
    return 1;
  }
  *heap_var = 99;

  printf("Memory Layout of This Process:\n");
  printf("==============================\n\n");

  /*
   * Note: Casting a function pointer to void* is not strictly defined
   * by C11, but is supported on POSIX systems and all common platforms.
   * We use a union-based approach or pragma to silence warnings.
   */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  printf("  Code  (main):     %p\n", (void *)(uintptr_t)main);
#pragma GCC diagnostic pop

  printf("  ROData (string):  %p\n", (const void *)string_literal);
  printf("  Data  (global):   %p\n", (void *)&global_initialized);
  printf("  BSS   (uninit):   %p\n", (void *)&global_uninitialized);
  printf("  Heap  (malloc):   %p\n", (void *)heap_var);
  printf("  Stack (local):    %p\n", (void *)&local_var);

  printf("\nObservations:\n");
  printf("  - Code and ROData are at low addresses (read-only segments)\n");
  printf("  - Data and BSS are nearby (global variable segments)\n");
  printf("  - Heap is above data (grows upward)\n");
  printf("  - Stack is at high addresses (grows downward)\n");

  free(heap_var);
  return 0;
}
