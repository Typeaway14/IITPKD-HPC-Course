//Minimum of N Elements
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define N 15

int main(int argc, char *argv[]){

	int rank, size;
	int localMin, globalMin;
	int data[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_rank(MPI_COMM_WORLD, &size);

	srand(rank);
	for(int i=0;i<N; i++){ data[i]=rand()%1000; }

	// Find Local Maximum
	localMin=data[0];
	for(int i=1; i< N; i++){ if(data[i]<localMin){ localMin=data[i]; }}
	
	//MPI_Reduce
	MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	if(rank ==0){
		printf("\n Global Minimum is: %d  ", globalMin);
		for(int i=0; i<N;i++) { printf("%d ", data[i]); }
	}
	if(rank ==1){
                for(int i=0; i<N;i++) { printf("%d ", data[i]); }
        }
	if(rank ==2){
                for(int i=0; i<N;i++) { printf("%d ", data[i]); }

        }
        if(rank ==3){
                for(int i=0; i<N;i++) { printf("%d ", data[i]); }
        }

	MPI_Finalize();

	return 0;
}
