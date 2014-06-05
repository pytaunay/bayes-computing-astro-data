#!/bin/bash

NSAMPLE=100

for i in $(seq $NSAMPLE)
do
	TIMER=`./cache-demo | awk 'BEGIN { FS=":" }; {print $2}' | tr -d '\n'`
	echo $TIMER

done | awk '{ s1 += $1; s2 += $2} END {print "Good access time (s): ",s1/NR,"\nBad access time (s):",s2/NR}'
