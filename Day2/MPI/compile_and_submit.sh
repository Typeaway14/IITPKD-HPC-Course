#!/bin/bash

VALUE="$@"
module load gnu8
module load openmpi3
mpicc ./programs/$VALUE.c -o ./objects/$VALUE.o
JOB_ID=$(sbatch ./submit.sh $VALUE | grep -Eo "[0-9]+")
while squeue | grep $JOB_ID > /dev/null
do
	echo waiting...
done
cat ./outputs/output.$JOB_ID.out


