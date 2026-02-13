/*
 * starter.c
 * Chapter 01 — Hello Machine
 * Exercise 03 — Process Memory Explorer
 * Original exercise
 *
 * CONCEPT: Every running C program organizes memory into segments:
 *          text (code), rodata, data, BSS, heap, and stack.
 *          This program prints addresses from each segment.
 *
 * MEMORY: Variables declared at file scope go in data or BSS.
 *         Variables declared inside functions go on the stack.
 *         malloc() allocates from the heap.
 *         Function code lives in the text segment.
 *
 * INSTRUCTIONS: Fill in each TODO to print the address of a variable
 * from the specified memory segment. Use printf with %p format.
 */
#include <stdio.h>
#include <stdlib.h>

/* TODO: Declare an initialized global variable (goes in DATA segment) */
/* Example: int global_initialized = 42; */

/* TODO: Declare an uninitialized global variable (goes in BSS segment) */
/* Example: int global_uninitialized; */

/* TODO: Declare a const string pointer (pointer in DATA, string in RODATA) */
/* Example: const char *string_literal = "Hello"; */

int main(void) {
  /* TODO: Declare a local variable (goes on STACK) */
  /* TODO: Allocate memory with malloc (goes on HEAP) */

  printf("Memory Layout of This Process:\n");
  printf("==============================\n\n");

  /* TODO: Print address of main (CODE/TEXT segment) */
  printf("  Code  (main):     TODO\n");

  /* TODO: Print address of string literal (RODATA segment) */
  printf("  ROData (string):  TODO\n");

  /* TODO: Print address of initialized global (DATA segment) */
  printf("  Data  (global):   TODO\n");

  /* TODO: Print address of uninitialized global (BSS segment) */
  printf("  BSS   (uninit):   TODO\n");

  /* TODO: Print address of malloc'd memory (HEAP segment) */
  printf("  Heap  (malloc):   TODO\n");

  /* TODO: Print address of local variable (STACK segment) */
  printf("  Stack (local):    TODO\n");

  /* TODO: Free the heap memory */

  return 0;
}
