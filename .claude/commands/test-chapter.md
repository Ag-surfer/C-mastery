---
description: Run full validation on a chapter. Usage: /test-chapter 6
---
Read CLAUDE.md. Validate chapter $ARGUMENTS:

1. Compile every .c file with all flag sets
2. Run every test harness (solution.c must pass, starter.c must fail ≥1 test)
3. Verify assembly annotations match gcc -S output
4. Check README has all 5 layers
5. Verify Excalidraw JSON files are valid
6. Check K&R credits on adapted exercises
7. Report results with pass/fail for each check
