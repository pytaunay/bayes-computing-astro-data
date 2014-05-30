CC=gcc
EXEC=single

CFLAGS=-fopenmp -std=c99 

all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $^ -o $@
