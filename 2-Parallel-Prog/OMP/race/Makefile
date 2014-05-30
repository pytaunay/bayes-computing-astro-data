CC=gcc
EXEC=race

CFLAGS=-fopenmp -std=c99 -g

all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $^ -o $@
