# C-Mastery — Project Instructions

## What Is This Project
C-Mastery is a 24-chapter C programming curriculum that teaches C as a mental model of the machine. Every chapter includes annotated code examples, gcc -S assembly output analysis, Excalidraw memory layout diagrams, and exercises drawn from K&R 2nd Edition plus original material. Hosted on GitHub with GitHub Pages documentation site.

## Teaching Philosophy
C is not a language you learn — it's a mental model of the machine. Every line of C maps to assembly instructions, memory addresses, and CPU register operations. A student who doesn't understand what the machine does when they write `int x = 5;` has not learned C.

## 5-Layer Teaching Structure (EVERY chapter must have all 5)
1. CONCEPT — What the C11 standard says (with K&R 2nd Ed references)
2. ASSEMBLY — What `gcc -S -O0` produces (x86-64 AT&T syntax, annotated)
3. MEMORY — Where bytes live (stack, heap, data, text segments with addresses)
4. DIAGRAM — Excalidraw visual of memory layout / data flow (2-3 per chapter)
5. PITFALL — What goes wrong and why (undefined behavior, leaks, overflows)

## K&R Integration Rules
- Every chapter references specific K&R 2nd Edition sections
- K&R exercises are adapted with assembly/memory analysis extensions
- K&R code examples are used as starting points, then extended with:
  - Assembly output annotations (gcc -S -O0)
  - Memory layout comments (which segment, stack offset)
  - sizeof/alignof verification in printf statements
  - Valgrind / AddressSanitizer verification
- Credit K&R in every adapted exercise: "Adapted from K&R Exercise X-Y"
- Add "Going Beyond K&R" sections showing what K&R couldn't cover (threads, sanitizers, modern tooling)

## Architecture Rules
- This is a STATIC GitHub project. No backend. No server. Just .c files, .md files, Makefiles, and diagrams.
- All code must compile with: gcc -Wall -Wextra -Werror -std=c11 -pedantic
- All code must pass: -fsanitize=address,undefined (on supported platforms)
- USE AGENT TEAMS for chapter generation (not subagents)
- The Curriculum Architect is the Team Lead, specialist agents are Teammates
- Teammates communicate directly with each other via messaging
- Shared task list coordinates all parallel work

## MCP Usage Rules
- ALWAYS use sequential thinking for chapter planning, exercise sequencing, curriculum dependencies
- ALWAYS use context7 when referencing Clang/gcc flags, AddressSanitizer options, lldb commands, POSIX APIs
- Use `gh` CLI (not GitHub MCP) for branch management, PRs, and issues — saves ~13,000 tokens
- Use Claude Code built-in tools (bash, view, create_file, str_replace) for all file operations — no Filesystem MCP needed
- Use memory MCP to track chapter completion and K&R exercise mapping across sessions
- Run `/context` periodically to check MCP token usage — keep under 6,000 tokens

## Agent Definitions
Agent definitions live in .claude/agents/*.md
- curriculum-architect.md — Team Lead (Opus 4.6) — plans, coordinates, never codes
- code-author.md — Writes annotated C examples (Sonnet 4.5)
- asm-analyst.md — Generates gcc -S output, annotates instructions (Sonnet 4.5)
- exercise-builder.md — Creates exercises + test harnesses + solutions (Sonnet 4.5)
- doc-writer.md — Writes lesson READMEs + Excalidraw diagrams (Sonnet 4.5)
- reviewer-qa.md — Compiles, runs sanitizers, checks pedagogy (Haiku 4.5)

## Code Style
- C: clang-format (LLVM style), C11 standard, -Wall -Wextra -Werror -pedantic
- Type hints in all C function signatures (parameter names always included)
- Every example file has a header comment block: filename, chapter, K&R reference, concept
- Assembly annotations use // ASSEMBLY: prefix in comments
- Memory annotations use // MEMORY: prefix in comments
- Every struct must have a comment showing sizeof, alignof, and padding bytes
- HOOKS ENFORCE THIS: clang-format auto-runs on every .c/.h file write, gcc compile check runs before commits

## Skills (Auto-Loaded Domain Knowledge)
Skills in .claude/skills/ are automatically loaded when Claude works on matching files:
- c-style-guide — loads when editing any .c or .h file
- lesson-template — loads when editing any chapter README.md
- exercise-patterns — loads when editing files in exercises/ directories
- assembly-annotation — loads when editing files with assembly comments or .s files
- memory-safety — loads when editing code with malloc/free/pointers
- makefile-patterns — loads when editing Makefile or *.mk files
- excalidraw-diagrams — loads when creating/updating .excalidraw files
- knr-exercises — loads when working on exercises mapped from K&R 2nd Edition

## Chapter Structure
```
chapters/NN-chapter-name/
├── README.md           # Lesson content (5-layer structure + K&R references)
├── examples/           # Annotated code examples
│   ├── example_01.c    # Primary concept demonstration
│   ├── example_02.c    # Extended / edge case exploration
│   └── example_03.c    # Assembly-focused example (gcc -S output in comments)
├── exercises/          # Student exercises
│   ├── exercise_01/    # Each exercise is a directory
│   │   ├── README.md   # Problem statement + K&R reference
│   │   ├── starter.c   # Skeleton code (compiles but fails tests)
│   │   ├── solution.c  # Reference solution (never shown to student first)
│   │   ├── test.c      # Test harness (auto-grading)
│   │   └── Makefile     # Build + test commands
│   └── exercise_02/
├── diagrams/           # Excalidraw JSON files
│   ├── memory-layout.excalidraw
│   └── stack-frame.excalidraw
└── Makefile            # Build all examples, run all tests
```

## Excalidraw Color Palette
| Element | Stroke | Fill | Usage |
|---|---|---|---|
| Stack | #d32f2f | #ffebee | Stack frames, local variables, return addresses |
| Heap | #388e3c | #e8f5e9 | malloc'd memory, dynamic allocations |
| Code/Text | #1976d2 | #e3f2fd | .text segment, function code |
| Data/BSS | #f57c00 | #fff3e0 | .data (initialized), .bss (uninitialized) |
| Pointers | #333333 | — | Arrows showing pointer references |
| Registers | #7b1fa2 | #f3e5f5 | CPU registers (%rax, %rsp, etc.) |
| Padding | #9e9e9e | #f5f5f5 | Struct padding bytes, alignment gaps |
| UB/Danger | #d32f2f | #ffcdd2 | Undefined behavior, buffer overflows |

## Compilation Standards
Every .c file must compile with ALL of these (using Apple Clang 17 via /usr/bin/gcc):
```bash
gcc -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g    # Debug build
gcc -Wall -Wextra -Werror -std=c11 -pedantic -O2        # Optimized build
gcc -fsanitize=address,undefined -g                      # Sanitizer build (native on Clang 17 + Apple Silicon)
gcc -S -O0 -fno-asynchronous-unwind-tables              # Assembly output (x86-64 or ARM64 depending on platform)
```
Note: On this machine, `gcc` is Apple Clang 17 (aliased by Xcode). Assembly output will be
ARM64 (AArch64) on Apple Silicon, not x86-64. Annotations must reflect actual Clang output.
Use `lldb` instead of `gdb` for debugging. Use `-fsanitize=address,undefined` instead of Valgrind.

## Development Workflow
1. Always read this file first
2. Check PROGRESS.md for current chapter and what's been built
3. Use sequential thinking for planning
4. Create a branch for the chapter (via `gh` CLI)
5. Spawn Agent Team for chapter generation
6. Team Lead coordinates — NEVER codes directly (delegate mode)
7. Each teammate works on their domain in parallel
8. Reviewer/QA validates all output
9. Commit, create PR (via `gh pr create`)
10. Update PROGRESS.md
