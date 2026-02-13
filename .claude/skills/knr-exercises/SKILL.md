---
name: knr-exercises
description: K&R 2nd Edition exercise mapping and adaptation rules. Auto-loads when working on K&R-derived exercises.
globs: ["**/exercises/**/README.md"]
---

## K&R Exercise Adaptation Rules
1. ALWAYS credit the source: "Adapted from K&R Exercise X-Y"
2. Preserve the original problem intent
3. EXTEND with C-Mastery layers:
   - Add assembly analysis requirement: "Examine the gcc -S output of your solution"
   - Add memory question: "Where does your data live? Stack or heap? Why?"
   - Add safety requirement: "Your solution must compile with -fsanitize=address,undefined"
4. Add modern context where K&R is outdated:
   - K&R uses gets() → require fgets()
   - K&R uses implicit int → require explicit types
   - K&R omits const → require const where appropriate

## K&R Chapter-to-C-Mastery Mapping

### K&R Chapter 1: A Tutorial Introduction
Maps to C-Mastery Chapters 1, 3, 9, 10
Key exercises:
- Ex 1-1 through 1-8: Basic I/O → Ch 1 (Hello Machine)
- Ex 1-9 through 1-12: Character processing → Ch 7 (Control Flow)
- Ex 1-13, 1-14: Histograms → Ch 9 (Arrays)
- Ex 1-15 through 1-24: Functions, strings → Ch 10 (Strings)

### K&R Chapter 2: Types, Operators, Expressions
Maps to C-Mastery Chapters 4, 5, 18
- Ex 2-1: Type ranges → Ch 4 (Types)
- Ex 2-2 through 2-5: String/char operations → Ch 5 (Operators)
- Ex 2-6 through 2-10: Bitwise → Ch 18 (Bitwise)

### K&R Chapter 3: Control Flow
Maps to C-Mastery Chapter 7
- Ex 3-1 through 3-6: All mapped to Ch 7

### K&R Chapter 4: Functions and Program Structure
Maps to C-Mastery Chapters 8, 14, 15
- Ex 4-1 through 4-2: Functions → Ch 8
- Ex 4-3 through 4-14: Calculator project → Ch 8 (progressive build)
- Ex 4-14: Swap macro → Ch 14 (Preprocessor)

### K&R Chapter 5: Pointers and Arrays
Maps to C-Mastery Chapters 6, 9, 20, 21
- Ex 5-1 through 5-5: Pointer basics → Ch 6
- Ex 5-6 through 5-10: Arrays/strings → Ch 9
- Ex 5-11 through 5-12: Pointers to functions → Ch 21
- Ex 5-13 through 5-20: Command-line args, sorting → Ch 20

### K&R Chapter 6: Structures
Maps to C-Mastery Chapters 11, 19
- Ex 6-1: getword → Ch 11
- Ex 6-2 through 6-6: Trees, hash tables → Ch 19

### K&R Chapter 7: Input and Output
Maps to C-Mastery Chapter 13
- Ex 7-1 through 7-9: All mapped to Ch 13

### K&R Chapter 8: The UNIX System Interface
Maps to C-Mastery Chapters 12, 23
- Ex 8-1 through 8-5: System calls → Ch 23
- Ex 8-6 through 8-8: Memory allocator → Ch 12
