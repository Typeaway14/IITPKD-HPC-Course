#!/bin/bash

#Author:Typeaway14
#Date:20th June,2023
#Reason: Automate the compilation and job submission process on the PARAMvidya 

VALUE="$@"
JOB_ID=$(sbatch submit.sh $VALUE | grep -Eo "[0-9]+")
while squeue | grep $JOB_ID > /dev/null
do
	echo "[$JOB_ID]waiting..."
done
cat ./outputs/output.$JOB_ID.out


