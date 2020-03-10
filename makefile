CC=gcc
CFLAGS=-Wall -Wextra -O2 -g

all: test

test: test.o readline.o avl.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm *.o
