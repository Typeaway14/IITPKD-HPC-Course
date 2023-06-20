#include<stdio.h>
#include<omp.h>

int main()
{
	omp_set_num_threads(4);
	#pragma omp parallel
	{
		#pragma omp master
		{
			printf("www.openmp.org Max Threads:%d\n", omp_get_thread_num());
		}
		printf("Welcome to HPC %d\n",omp_get_thread_num());
		#pragma omp barrier
		printf("Are you able to understand the concepts? %d\n", omp_get_thread_num());
	}
	#pragma omp cancel
	printf("And we're done with the parallel part of the code\n");
	return 0;
}
