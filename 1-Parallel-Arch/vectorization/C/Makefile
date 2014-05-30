CC=icc

VECFLAGS=-O1 -xHost -std=c99 -fopenmp
NOVECFLAGS=-O1 -std=c99 -fopenmp

INC=
LIB=

all:
	"Run:"
	" - 'make vec' to create the vectorized executable"
	" - 'make novec' to create the non vectorized executable"

vec: 
	$(CC) $(VECFLAGS) main.c -o $@

novec:
	$(CC) $(NOVECFLAGS) main.c -o $@
