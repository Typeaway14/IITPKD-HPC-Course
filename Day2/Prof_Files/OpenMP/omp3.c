#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int x=0;
int main(int argc, char *argv[])
{
	int *A, *B, dotProduct=0;
	int size=atoi(argv[1]);
	int numThreads=atoi(argv[2]);
	A=(int *)malloc(sizeof(int)*size);
	B=(int *)malloc(sizeof(int)*size);
	for(int i=0;i<size;i++){
		A[i]=1;
		B[i]=1;
	}
	omp_set_num_threads(numThreads);
	int *parSum;
	int maxThreads=omp_get_max_threads();
	parSum=(int *)calloc(sizeof(int), maxThreads*16);
	double start_time=omp_get_wtime();
	#pragma omp parallel 			// may consider reduction(+:dotProduct) 
	{
		int j=omp_get_thread_num();	
		for(int i=j ;i<size; i=i+maxThreads){
			//#pragma omp critical // may consider critical section
			// dotProduct+=A[i]*B[i];
			parSum[j*16]+=A[i]*B[i];     // Suffer with false sharing
		}
	}
		for(int i=0;i<maxThreads;i++){ dotProduct+=parSum[i*16]; } // suffer with false sharing
	double end_time=omp_get_wtime();
	double elapsed_time=(end_time-start_time)*1000; // Time in Milli Seconds
	printf("Result is %d  and time is %lf \n", dotProduct, elapsed_time);
return 0;
}
