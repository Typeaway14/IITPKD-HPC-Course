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
		a[i]=2.0*step;
	}
	omp_set_num_threads(numth);
	int maxthreads=omp_get_max_threads();
	double **sum;
	sum=(double **)calloc(sizeof(double),maxthreads);
	for(i=0;i<maxthreads;i++)
	{
		sum[i]=(double *)calloc(sizeof(double), 8);
	}
	start=omp_get_wtime();
	#pragma omp parallel
	{
		int tid=omp_get_thread_num();
		for(int j=tid;j<size;j=j+maxthreads)
		{
			sum[tid][0]+=a[j];
		}
	}
	for(i=0;i<maxthreads;i++)
	{
		//printf("%lf\n",sum[i]);
		area+=sum[i][0];
	}
	end=omp_get_wtime();
	printf("Area is %lf  and time is  %lf\n", area, (end-start));
return 0;
}
