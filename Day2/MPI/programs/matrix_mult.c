#include<stdio.h>
#include<mpi.h>

#define MAT_SIZE 16

void MM(int A[MAT_SIZE][MAT_SIZE], int B[MAT_SIZE][MAT_SIZE],int C[MAT_SIZE][MAT_SIZE], int start_row,int end_row)
{
	for(int i=start_row; i<end_row;i++)
	{
		for(int j=0;  j<MAT_SIZE;j++)
		{
			C[i][j]=0;
			for(int k=0;k<MAT_SIZE;k++)
			{
				C[i][j] += A[i][k]*B[i][j];
			}
		}
	}
}

int main(int argc, char* argv[])
{
	int rank,size;
	int limit; 
	double start_time=0.0,end_time=0.0,elapsed=0.0;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);		

	double A[MAT_SIZE][MAT_SIZE];
	double B[MAT_SIZE][MAT_SIZE];
	double C[MAT_SIZE][MAT_SIZE];
	limit = MAT_SIZE/size;
	double Amat[limit][MAT_SIZE];
	double Cmat[limit][MAT_SIZE];

	for(int i = 0; i < MAT_SIZE; i++)
	{
		for(int j = 0; j < MAT_SIZE; j++)
		{
			A[i][j] = 2;
			B[i][j] = 3;
			C[i][j] = 1; // initializing C to 1 
			
		}
	}
	
	start_time = MPI_Wtime();
	MPI_Scatter(A,limit,MPI_INT,Amat,limit,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(B,MAT_SIZE*MAT_SIZE,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	int sum=0;
	for(int i=0; i < limit;i++)
	{
		for(int j=0; j < MAT_SIZE; j++)
		{
			for(int k=0; k < MAT_SIZE; k++)
			{
				sum += Amat[i][k]*B[k][j];
			}
			Cmat[i][j]=sum;
			sum = 0;		
		}
	}

	MPI_Gather(Cmat,limit*MAT_SIZE,MPI_INT,C,limit*MAT_SIZE,MPI_INT,0,MPI_COMM_WORLD);
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
