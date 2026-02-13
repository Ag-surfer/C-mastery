CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g
SANITIZE = -fsanitize=address,undefined
ASMFLAGS = -S -O0 -fno-asynchronous-unwind-tables

# Single-file examples (exclude linking_helper.c and linking_main.c)
SINGLE_SRCS = examples/preprocessor_stages.c examples/object_file_anatomy.c
SINGLE_BINS = $(SINGLE_SRCS:.c=)

# Multi-file linking demo
LINKING_SRCS = examples/linking_main.c examples/linking_helper.c
LINKING_BIN = examples/linking_demo

.PHONY: all clean test asm

all: $(SINGLE_BINS) $(LINKING_BIN)

# Single-file examples: one .c -> one binary
$(SINGLE_BINS): examples/%: examples/%.c
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $<

# Multi-file linking demo: two .c files -> one binary
$(LINKING_BIN): $(LINKING_SRCS) examples/linking_helper.h
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $(LINKING_SRCS)

# Generate assembly output for all source files
asm: $(SINGLE_SRCS) $(LINKING_SRCS)
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
	rm -f examples/*.o examples/*.s $(SINGLE_BINS) $(LINKING_BIN)
	rm -rf examples/*.dSYM
	@for dir in exercises/exercise_*/; do \
		if [ -f "$$dir/Makefile" ]; then \
			$(MAKE) -C "$$dir" clean; \
		fi; \
	done
