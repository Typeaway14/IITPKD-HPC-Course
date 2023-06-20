#!/bin/bash

#Author:Typeaway14
#Date:20th June,2023
#Reason: Automate the compilation and job submission process on the PARAMvidya 

#SBATCH --job-name=mpi_job
#SBATCH --account=cdac
#SBATCH --time=00:10:00
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4
#SBATCH --mem-per-cpu=1GB
#SBATCH --partition=standard
#SBATCH --error=errors/error.%J.err
#SBATCH --output=outputs/output.%J.out

VALUE="$1"
THREADS="$2"
ARGUMENTS="${@:3}"

#Loading OpemMPI environment modules
module load gnu8
module load openmpi3

#Execute the following command
mpirun -np $THREADS ./objects/$VALUE.o
