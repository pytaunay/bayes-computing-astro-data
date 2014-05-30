CC=gcc
EXEC=cache-demo

CFLAGS=-O3 -std=c99 -fopenmp

INC=
LIB=

all: $(EXEC)

$(EXEC): main.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
