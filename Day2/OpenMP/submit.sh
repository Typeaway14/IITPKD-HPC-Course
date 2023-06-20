#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks-per-node=16
#SBATCH --error=errors/error.%J.err
#SBATCH --output=outputs/output.%J.out
#SBATCH --time=00:10:00
#SBATCH --partition=cpu

VALUE="$1"
ARGUMENTS="${@:2}"

#Code will be written here
module load gnu8
module load openmpi3

#Execute the following command
./objects/$VALUE.o $ARGUMENTS
