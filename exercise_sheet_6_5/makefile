SHELL = /bin/bash

CC = gcc
CFLAGS = -std=gnu17 -O3 -march=native -fopenmp -DNDEBUG

all : prog

prog : main.c your_code.c
	$(CC) $(CFLAGS) main.c -o $@

.PHONY : clean
clean :
	rm -f prog