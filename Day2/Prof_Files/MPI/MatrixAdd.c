// MPI Program for Matrix Addition
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define N 16

void MA(int A[N][N], int B[N][N], int C[N][N], int start_row, int end_row){
	for(int i=start_row;i<end_row;i++){
		for(int j=0;j<N;j++){ 
			C[i][j]=A[i][j]+B[i][j];
		}
	}
}
int main(int argc, char *argv[]){
	
	int rank, size, flag;
	int A[N][N], B[N][N], C[N][N];
	int subMatrixSize, start_row, end_row;
	double startTime, endTime;

	MPI_Init(&argc, &argv);
	MPI_Initialized(&flag);
	if(flag==0){ MPI_Init(&argc, &argv); }
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//size of sub-Matrix
	subMatrixSize=N/size;
	start_row=rank*subMatrixSize;
	end_row=start_row+subMatrixSize;

	//Initialization
	if(rank==0){
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				A[i][j]=1;
				B[i][j]=2;
			}
		}
	}
	startTime=MPI_Wtime();
	//Scatter Matrix A to all processes
	MPI_Scatter(A,subMatrixSize*N,MPI_INT,A[start_row],subMatrixSize*N,MPI_INT, 0, MPI_COMM_WORLD);
	
	//Scatter Matrix B to all processes
        MPI_Scatter(B,subMatrixSize*N,MPI_INT,B[start_row],subMatrixSize*N,MPI_INT, 0, MPI_COMM_WORLD);

	//Perform MA
	MA(A,B,C, start_row, end_row);

	//Gather the result from all processes
	MPI_Gather(C[start_row],subMatrixSize*N,MPI_INT,C,subMatrixSize*N,MPI_INT,0,MPI_COMM_WORLD);
	endTime=MPI_Wtime();
	if(rank==0)
	{
		printf("\n Execution Time %lf", endTime-startTime);
		for(int i=0;i<N;i++){
			printf("\n");
                        for(int j=0;j<N;j++){
                               printf("%d  ", C[i][j]);
                        }
                }
	
	}
		
	MPI_Finalize();
	return 0;
}
