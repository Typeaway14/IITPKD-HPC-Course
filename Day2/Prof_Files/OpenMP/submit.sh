#!/bin/bash
#SBATCH -N 1 
#SBATCH --ntasks-per-node=48
#SBATCH --error=errs/job.%J.err 
#SBATCH --output=outputs/job.%J.out
#SBATCH --time=00:10:00
#SBATCH --partition=cpu

# Write your code below
module load gnu8
module load openmpi3

# Execute the following command
./MatrixOP.o 2048 48


