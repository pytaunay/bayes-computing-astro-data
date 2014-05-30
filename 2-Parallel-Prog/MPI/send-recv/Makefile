CC=mpicc
EXEC=send-recv
CFLAGS=-std=c99

all: $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $^ -o $@ 
