#include<stdio.h>
#include<omp.h>

int main()
{
	int maxthreads = omp_get_max_threads();
	omp_set_num_threads(4);
	printf("Maximum number of threads in the shared memory environment is %d\n",maxthreads);
	#pragma omp parallel
	{
		printf("Welccome to HPC and AI Summer School, Hostel by IIT Palakkad %d\n",omp_get_thread_num());
	}
	return 0;
}

