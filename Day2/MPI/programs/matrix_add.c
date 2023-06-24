#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define MAT_SIZE 32
int main(int argc, char *argv[])
{
	int rank, size;
	int limit;
	double start_time=0.0,end_time=0.0;
	double elapsed=0.0;
	while(MPI_Init(&argc,&argv) != MPI_SUCCESS)
	{
		MPI_Init(&argc,&argv); 
	}
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int A[MAT_SIZE][MAT_SIZE];
	int B[MAT_SIZE][MAT_SIZE];
	limit = MAT_SIZE/size;
	int Arows[limit][MAT_SIZE];
	int Brows[limit][MAT_SIZE];
	for(int i =0; i<MAT_SIZE; i++)
	{
		for(int j=0; j<MAT_SIZE; j++)
		{
			A[i][j] = 2;
			B[i][j] = 3;
		}
	}
	start_time=MPI_Wtime();
	MPI_Scatter(A,limit,MPI_INT,Arows,limit,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(B,limit,MPI_INT,Brows,limit,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < limit ; i++)
	{
		for(int j=0; j < MAT_SIZE;j++)
		{
			Brows[i][j] = Arows[i][j] + Brows[i][j];
		}
	}
	MPI_Gather(Brows,limit,MPI_INT,B,MAT_SIZE,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	end_time=MPI_Wtime();
	if(rank == 0)
	{
		elapsed = end_time - start_time;
		printf("Elapsed time = %4.2lf seconds. \n",elapsed);
		for(int i=0; i<MAT_SIZE;i++)
		{
			for(int j=0; j<MAT_SIZE;j++)
			{
				printf(" %d ",B[i][j]);
			}
			printf("\n");
		}
	}

	return 0;
}
