CC=mpicc

EXEC=gemv

CFLAGS=-std=c99

all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(EXEC)
