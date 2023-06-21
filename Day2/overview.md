# OpenMP and MPI

## Introduction to OpenMP and MPI

> Speaker for the day:[Praveen Kumar Alapati](https://mahindrauniversity.edu.in/faculty/dr-praveen-kumar-alapati/):

### Session 1:
 - Talks about Memory Heirarchy   
 - Paging and Page Sizes
 - Block transfer from secondary to main memory with DMA
 - Inclusive and exclusive caches

### Session 2:
- Shows "hello world" program parallelization with OpenMP Pragma commands
> All OpenMP programs found [here](../Day2/OpenMP/programs/)
- Explains the following parallelized algorithms(`#pragma` directives):
    - Basic printing commands and controlling flow of parallel program
    - Matrix Multiplication
    - Matrix Addition
- Hands on session to implement these programs.
- Brief of UMA and NUMA to transition from OpenMP to MPI

### Session 3:
- Explains about distributed & Hybrid memory architectures
- [Collective Communication Routines](https://mpitutorial.com/tutorials/mpi-broadcast-and-collective-communication/)
    - Broadcast
    - Scatter
    - Gather
    - Reduce
- MPI Program structure
> All MPI programs found [here](../Day2/MPI/programs)
- Explains following programs with MPI:
    - Sending and receiving data between multiple nodes
    - Matrix addition with `MPI_Scatter` and `MPI_Gather` 
    - Matrix multiplication with `MPI_Scatter`, `MPI_Bcast` and `MPI_Gather`
   
### Assignment:
- Implement the following parallel algorithms with MPI
    - Matrix Addition
    - Matrix Multiplication
    - Merge Sort
    - Prefix Sum Problem

