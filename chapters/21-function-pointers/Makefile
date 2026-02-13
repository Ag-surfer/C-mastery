CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g
SANITIZE = -fsanitize=address,undefined
ASMFLAGS = -S -O0 -fno-asynchronous-unwind-tables

EXAMPLES = $(wildcard examples/*.c)
EXAMPLE_BINS = $(EXAMPLES:.c=)

.PHONY: all clean test asm

all: $(EXAMPLE_BINS)

examples/%: examples/%.c
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $<

asm: $(EXAMPLES)
	@for src in $^; do \
		$(CC) $(ASMFLAGS) -o $${src%.c}.s $$src; \
	done

test:
	@for dir in exercises/exercise_*/; do \
		if [ -f "$$dir/Makefile" ]; then \
			$(MAKE) -C "$$dir" test; \
		fi; \
	done

clean:
	rm -f examples/*.o examples/*.s $(EXAMPLE_BINS)
	@for dir in exercises/exercise_*/; do \
		if [ -f "$$dir/Makefile" ]; then \
			$(MAKE) -C "$$dir" clean; \
		fi; \
	done
