
#include<stdio.h>
#include<omp.h>
int main()
{
	#pragma omp parallel
	{
		printf("\n Hello World");
	}
	return 0;
}
