#!/bin/sh
#SBATCH -N 1 
#SBATCH --ntasks-per-node=1
#SBATCH --time=72:00:00 
#SBATCH --job-name=Test.melt
#SBATCH --error=./errors/job.%J.err_16_node_16 
#SBATCH --output=./outputs/job.%J.out_16_node_16 
#SBATCH --partition=standard 

#VALUE="$@"

module load gnu8
module load openmpi3


cd $SLURM_SUBMIT_DIR

mpirun -np $SLURM_NTASKS ./lmp_mpi -in in.melt -log ./logs/melt-Proc1-%J.log
