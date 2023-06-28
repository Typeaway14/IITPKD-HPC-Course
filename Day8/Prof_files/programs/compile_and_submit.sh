#!/bin/bash


#Author:Typeaway14
#Date:20th June,2023
#Reason: Automate the compilation and job submission process on the PARAMvidya 

ALL="$@"
VALUE=$1

JOB_ID=$(sbatch ./submit.sh $ALL | grep -Eo "[0-9]+")
while squeue | grep $JOB_ID > /dev/null
do
	echo "[$JOB_ID]waiting..."
done
cat ../outputs/output.$JOB_ID.out


