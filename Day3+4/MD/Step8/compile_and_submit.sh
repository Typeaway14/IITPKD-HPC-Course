#!/bin/bash

#Author:Typeaway14
#Date:20th June,2023
#Reason: REWRITTEN TO Automate the compilation and job submission process FOR PYTHON  on the PARAMvidya 

VALUE="$@"
module load gnu8
module load openmpi3
JOB_ID=$(sbatch submit.sh $VALUE | grep -Eo "[0-9]+")
while squeue | grep $JOB_ID > /dev/null
do
	echo "[$JOB_ID]waiting..."
done
cat ./outputs/job.$JOB_ID.out_16_node_16


