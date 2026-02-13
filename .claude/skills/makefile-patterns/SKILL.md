---
name: makefile-patterns
description: Makefile conventions for C-Mastery chapters and exercises.
globs: ["**/Makefile", "**/*.mk"]
---

## Chapter Makefile Template
````makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g
SANITIZE = -fsanitize=address,undefined
ASMFLAGS = -S -O0 -fno-asynchronous-unwind-tables

EXAMPLES = $(wildcard examples/*.c)
EXAMPLE_BINS = $(EXAMPLES:.c=)
EXAMPLE_ASM = $(EXAMPLES:.c=.s)

.PHONY: all test asm clean

all: $(EXAMPLE_BINS)

%: %.c
	$(CC) $(CFLAGS) $(SANITIZE) $< -o $@

test:
	@for dir in exercises/*/; do \
		echo "Testing $$dir..."; \
		$(MAKE) -C "$$dir" test; \
	done

asm: $(EXAMPLE_ASM)

%.s: %.c
	$(CC) $(ASMFLAGS) $< -o $@

clean:
	rm -f $(EXAMPLE_BINS) $(EXAMPLE_ASM)
	@for dir in exercises/*/; do $(MAKE) -C "$$dir" clean; done
````
