CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O0 -g
SANITIZE = -fsanitize=address,undefined

.PHONY: all test clean

all: starter solution test_harness

starter: starter.c
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $<

solution: solution.c
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $<

test_harness: test.c
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $<

test: solution test_harness
	./test_harness

clean:
	rm -f starter solution test_harness
	rm -rf *.dSYM
