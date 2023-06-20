#include<stdio.h>
#include<mpi.h>
int main(int argc, char *argv[])
{
	int rank, size, version, subversion;
	int data;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank==0)//Process 0 sends data to process 1
	{
		data=9999;
		MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("Process 0 sent data: %d \n",data);
	}
	else if(rank==1)//Process 1 receive sdata from process 0
	{
		MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process 1 received data: %d \n", data);	
	}
	if(rank==0)
	{
		MPI_Get_version(&version, &subversion);
		printf("\n MPI Version: %d.%d",version,subversion);
	}
	
	MPI_Finalize();
}
