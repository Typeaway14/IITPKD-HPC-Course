#!/bin/bash

VALUE="$@"

gcc ./programs/$VALUE.c -o ./objects/$VALUE.o -fopenmp
JOB_ID=$(sbatch ./submit.sh $VALUE | grep -Eo "[0-9]+")
while squeue | grep $JOB_ID > /dev/null
do
	echo waiting...
done
cat ./outputs/output.$JOB_ID.out


