---
description: Generate and verify assembly annotations for a chapter's examples. Usage: /annotate-asm 6
---
Read CLAUDE.md. Annotate assembly for chapter $ARGUMENTS:
1. For each .c file in the chapter's examples/:
   a. Run gcc -S -O0 -fno-asynchronous-unwind-tables
   b. Read the .s output
   c. Add/verify // ASSEMBLY: comments in the .c file
   d. Verify stack offsets and register assignments
2. Create example_NN_asm.c focused on assembly walkthrough
