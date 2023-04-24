#!/bin/bash

#SBATCH --job-name=q2q3
#SBATCH --nodes=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=2GB 
#SBATCH --time=01:00:00 
#SBATCH --verbose
#SBATCH --output=slurm_hw4q2q3.out
 

mpiexec ./int_ring 10
mpiexec ./int_ring 100
echo 'Question 3 below'
mpiexec ./mpi_scan
