---
description: Deep pedagogical review of a chapter. Usage: /deep-review 6
---
Read CLAUDE.md. Deep review chapter $ARGUMENTS:

1. Load all chapter files into context
2. For each example: compile, verify assembly annotations match gcc -S output
3. For each exercise: verify starter compiles but fails, solution passes all tests
4. For README: verify 5-layer structure, cross-reference with previous chapters
5. Check difficulty progression is appropriate
6. Verify K&R references are accurate
7. Check no forward references to later chapters
8. Report findings with severity
