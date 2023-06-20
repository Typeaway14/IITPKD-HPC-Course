#include<omp.h>
#include<stdio.h>
/*int main()
{
	#pragma omp parallel
	{
	printf("Welcome to HPC and AI Summer School, Hosted by IIT Palakkad \n");
	}
	return 0;
}*/
/*int main()
{
		int maxThreads=omp_get_max_threads();
		omp_set_num_threads(4);
		printf(" Maximum number of threads in the shared memory environment is %d\n", maxThreads);
	        #pragma omp parallel
	        {
		printf("Welcome to HPC and AI Summer School, Hosted by IIT Palakkad %d\n", omp_get_thread_num());
		}
		return 0;
}*/
int main()
{
	omp_set_num_threads(4);	
	#pragma omp parallel
	{
			#pragma omp master 
			{
			      	printf("www.openmp.org Max Threads:%d\n", omp_get_thread_num()); 
			}
			printf("Welcome to HPC %d\n", omp_get_thread_num());
			#pragma omp barrier
			printf("Are you able to understand the concepts! %d\n", omp_get_thread_num());
	}
return 0;
}
