#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

void merge(int *arr, int *left, int left_size, int *right, int right_size){
	int i=0, j=0, k=0;

	while(i<left_size && j<right_size){ 
		if(left[i]<=right[j]){ arr[k++]=left[i++];  }
		else{ arr[k++]=right[j++]; }
	}
	while(i<left_size){ arr[k++]=left[i++];} 
	while(j<right_size){ arr[k++]=right[j++]; }
}
void mergeSort(int *arr, int size){
	if(size>1){
	int mid=size/2;
	int *left=(int *)malloc(mid*sizeof(int));
	int *right=(int *)malloc((size-mid)*sizeof(int));
	for(int i=0;i<mid;i++){ left[i]=arr[i];}
	for(int i=mid;i<size;i++){ right[i-mid]=arr[i]; }
	
	mergeSort(left, mid);
	mergeSort(right, size-mid);
	
	merge(arr, left, mid, right, size-mid);
	
	free(left);
	free(right);
	}
}
int main(int argc, char *argv[]){
	
	int rank, size;
	int *data=NULL;
	int *local_data=NULL;
	int *sorted_data=NULL;
	int local_size, global_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank==0){
		//Only on rank 0 generates data
		global_size=100;
		data=(int *)malloc(global_size*sizeof(int));

		srand(0);
		for(int i=0;i<global_size;i++){ data[i]=rand()%1000; }
	}

	//Broadcast the global size to all processes
	MPI_Bcast(&global_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Determine the local size
	local_size=global_size/size;

	//Allocate memory for local data
	local_data=(int *)malloc(local_size*sizeof(int));

	//Scatter the data from rank 0 to all processes
	MPI_Scatter(data, local_size, MPI_INT, local_data, local_size, MPI_INT, 0, MPI_COMM_WORLD);

	//Sort the local data
	mergeSort(local_data, local_size);
	
	//Gather the sorted data to rank 0
	if(rank==0){ sorted_data=(int *)malloc(global_size*sizeof(int)); }

	MPI_Gather(local_data, local_size, MPI_INT, sorted_data, local_size, MPI_INT,0, MPI_COMM_WORLD);	
	// Print Sorted Data
		if(rank==0){
		for(int i=0;i<global_size;i++){ printf("%d  ", sorted_data[i]); }
	}
	
	//Free Memory
	if(rank==0){
		free(data);
		free(sorted_data);
	}
	free(local_data);

	MPI_Finalize();
	return 0;
}

