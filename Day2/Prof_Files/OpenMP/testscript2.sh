#!/bin/bash
#SBATCH -N 1 
#SBATCH --ntasks-per-node=16
#SBATCH --error=errs/job.%J.err 
#SBATCH --output=outputs/job.%J.out
#SBATCH --time=00:10:00
#SBATCH --partition=cpu

# module load compiler/intel/2018.4
export OMP_NUM_THREADS=32
./omp1.o

#./MatrixOP 1024 1

