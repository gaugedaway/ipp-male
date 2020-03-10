CC=gcc
CFLAGS=-Wall -Wextra -O2 -g

all: test

test: avl.o test.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm *.o
