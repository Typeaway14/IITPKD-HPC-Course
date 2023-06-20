#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	double *a,area=0.0;
	double start, end;
	int i, size, numth;
	size=atoi(argv[1]);
	numth=atoi(argv[2]);
	a=(double *)calloc(sizeof(double), size);
	double step=2.0/(double)size;
	for(i=0;i<size;i++)
	{
		a[i]=step*2.0;
	}
	omp_set_num_threads(numth);
	int maxthreads=omp_get_max_threads();
	double sum;

	start=omp_get_wtime();
	#pragma omp parallel reduction(+:sum)
	{
		int tid=omp_get_thread_num();
		for(int j=tid;j<size;j=j+maxthreads)
		{
			sum+=a[j];
		}
	}
		area=sum;
	end=omp_get_wtime();
	printf("Area is %lf  and time is  %lf\n", area, (end-start));
return 0;
}
