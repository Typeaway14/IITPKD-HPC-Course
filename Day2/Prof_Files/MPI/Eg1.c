#include<stdio.h>
#include<mpi.h>

int main(int argc,char *argv[])
{
	int rank, size, len;
	char hostname[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Get_processor_name(hostname, &len);

	printf(" Hello World, rank:%d, size:%d  Processor Name: %s\n ", rank, size, hostname);

	MPI_Finalize();

	return 0;
}
