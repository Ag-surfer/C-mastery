---
description: Build a complete chapter with Agent Teams. Usage: /new-chapter 6 "Pointers"
---
Read CLAUDE.md and PROGRESS.md. Build chapter $ARGUMENTS using Agent Teams:

1. Parse arguments: first word = chapter number, rest = chapter title
2. Use sequential thinking to plan the chapter:
   - What K&R sections and exercises map to this chapter?
   - What concepts from previous chapters are prerequisites?
   - What 3-5 example files best demonstrate the concept?
   - What 3-5 exercises test understanding? (include K&R adaptations)
   - What 2-3 Excalidraw diagrams visualize the memory/data flow?
3. Create chapter directory: chapters/NN-chapter-name/
4. Create shared task list with dependencies
5. Spawn Agent Team: Code Author, ASM Analyst, Exercise Builder, Doc Writer
6. Enter delegate mode — coordinate only, never code directly
7. When all tasks complete, spawn Reviewer/QA for validation
8. If review passes: commit with descriptive message via `gh`
9. If review fails: fix issues and re-review
10. Update PROGRESS.md
11. Clean up team
