#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define  N 40

int main(int argc, char *argv[]){
	
	int rank, size;
	int *prefix=NULL;
	int *input=NULL;
	int *local_input=NULL, *local_prefix=NULL;
	int m;//local  size

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank==0){
		//Only on rank 0 generates data
		input=(int *)malloc(N*sizeof(int));
		prefix=(int *)malloc(N*sizeof(int));

		for(int i=0;i<N;i++){ input[i]=i+1; }
	}

	m=N/size;

	local_input=(int *)malloc(m*sizeof(int));
	local_prefix=(int *)malloc(m*sizeof(int));

	//Scatter Input to all processes
	MPI_Scatter(input,m, MPI_INT, local_input, m, MPI_INT, 0, MPI_COMM_WORLD);
	local_prefix[0]=local_input[0];
	for(int i=1;i<m;i++){ local_prefix[i]=local_prefix[i-1]+local_input[i]; }

	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(local_prefix, m, MPI_INT,prefix,m, MPI_INT, 0,  MPI_COMM_WORLD); 	
	MPI_Barrier(MPI_COMM_WORLD);	
	if(rank==0){
		for(int i=0;i<N;i++){
			printf(" %d", prefix[i]);
		}
	}
        MPI_Barrier(MPI_COMM_WORLD);
	//Free Memory
	if(rank==0){
		free(input);
		free(prefix);
	}
	free(local_input);
	free(local_prefix);
	MPI_Finalize();
	return 0;
}

