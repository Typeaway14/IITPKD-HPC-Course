#!/bin/bash

## Resource Request
#SBATCH --job-name=%J
#SBATCH --output=outputs/%Jout.txt
#SBATCH --error=errs/%Jerr.txt
#SBATCH --time=00:10:00
#SBATCH -N 1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --mem-per-cpu=500M

##./omp1.o
./MatrixOP.o.old 1000 16
