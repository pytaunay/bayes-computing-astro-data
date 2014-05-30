#!/bin/bash

#set -x

#VEC="-O1 -xHost -std=c99 -fopenmp -par-report3 -opt_report3"
#NO_VEC="-O1 -std=c99 -fopenmp -par-report3 -opt_report3"

#OUT_VEC=vec
#OUT_NO_VEC=no_vec

#CC=icc

#IN=main.c


# Compile
#$CC $IN -o $OUT_VEC $VEC
#$CC $IN -o $OUT_NO_VEC $NO_VEC

# Run
FILE_VEC=vectorized
if [[ -e $FILE_VEC ]]
then
	rm $FILE_VEC 
fi

FILE_NO_VEC=not_vectorized
if [[ -e $FILE_NO_VEC ]]
then
	rm $FILE_NO_VEC 
fi

# Results
for i in $(seq 1 200); do ./vec | grep -oE "[0-9]*\.[0-9]*"; done > vectorized
cat vectorized | awk '{sum += $1} END {print "Vectorized: ",sum/NR}'

for i in $(seq 1 200); do ./novec | grep -oE "[0-9]*\.[0-9]*"; done > not_vectorized
cat not_vectorized | awk '{sum += $1} END {print "Not vectorized: ",sum/NR}'
