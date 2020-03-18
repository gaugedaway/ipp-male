CC=gcc
CFLAGS=-Wall -Wextra -O2 -g

all: main

main: main.o readline.o avl.o commands.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm *.o
