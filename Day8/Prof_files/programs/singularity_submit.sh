#!/bin/bash
#SBATCH --job-name=torch-test
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=4GB
#SBATCH --gres=gpu:1
#SBATCH --time=00:05:00
#SBATCH --error=../errors/error.%J.err
#SBATCH --output=../outputs/output.%J.out

FILE=$1

module load singularity

singularity exec --nv ../python-va3.sif python3 $FILE
