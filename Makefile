CC = gcc
CFLAGS = -Wall -g -std=c11 -lm

all: build

build: quadtree

quadtree: quadtree.c
	$(CC) quadtree.c -o quadtree $(CFLAGS)

run: build
	./quadtree
val: build
	valgrind --leak-check=full ./quadtree

clean:
	rm -f *.o quadtree

.PHONY: clean run build
