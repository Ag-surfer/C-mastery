---
description: Generate annotated code examples for a chapter. Usage: /build-examples 6 "Pointers"
---
Read CLAUDE.md. Build examples only for chapter $ARGUMENTS:
1. Create examples/ directory with 3-5 annotated .c files
2. Every file has header block, ASSEMBLY:, MEMORY:, K&R: annotations
3. Run gcc -S to verify assembly annotations
4. Run gcc with sanitizers to verify memory safety
