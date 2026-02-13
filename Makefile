CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g
SANITIZE = -fsanitize=address,undefined

CHAPTERS = $(sort $(wildcard chapters/*/))

.PHONY: all test asm clean status

all:
	@for ch in $(CHAPTERS); do \
		echo "=== Building $$ch ==="; \
		$(MAKE) -C "$$ch" all 2>/dev/null || echo "  (no Makefile)"; \
	done

test:
	@passed=0; failed=0; \
	for ch in $(CHAPTERS); do \
		if [ -f "$$ch/Makefile" ]; then \
			echo "=== Testing $$ch ==="; \
			$(MAKE) -C "$$ch" test && passed=$$((passed+1)) || failed=$$((failed+1)); \
		fi; \
	done; \
	echo ""; echo "=== Results: $$passed passed, $$failed failed ==="

asm:
	@for ch in $(CHAPTERS); do \
		if [ -f "$$ch/Makefile" ]; then \
			$(MAKE) -C "$$ch" asm 2>/dev/null; \
		fi; \
	done

clean:
	@for ch in $(CHAPTERS); do \
		if [ -f "$$ch/Makefile" ]; then \
			$(MAKE) -C "$$ch" clean; \
		fi; \
	done

status:
	@echo "C-Mastery Build Status"; \
	echo "======================"; \
	total=$$(ls -d chapters/*/README.md 2>/dev/null | wc -l | tr -d ' '); \
	echo "Chapters with README: $$total / 24"; \
	exercises=$$(find chapters -name test.c 2>/dev/null | wc -l | tr -d ' '); \
	echo "Test harnesses: $$exercises"; \
	diagrams=$$(find chapters -name '*.excalidraw' 2>/dev/null | wc -l | tr -d ' '); \
	echo "Excalidraw diagrams: $$diagrams"
