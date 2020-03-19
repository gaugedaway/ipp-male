CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

all: forests

forests: main.o input.o avl.o commands.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm forests
	rm *.o
