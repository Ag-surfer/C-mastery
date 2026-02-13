---
description: Generate exercises for a chapter. Usage: /build-exercises 6
---
Read CLAUDE.md. Build exercises for chapter $ARGUMENTS:
1. Create exercises/ directory with 3-5 exercise subdirectories
2. Each has: README.md, starter.c, solution.c, test.c, Makefile
3. Include K&R adapted exercises where mapped
4. Progressive difficulty: ★☆☆ → ★★☆ → ★★★
5. Verify solution.c passes test.c, starter.c fails ≥1 test
