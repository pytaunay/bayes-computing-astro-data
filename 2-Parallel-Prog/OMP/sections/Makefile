CC=gcc
EXEC=sections

CFLAGS=-fopenmp -std=c99

all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $^ -o $@
