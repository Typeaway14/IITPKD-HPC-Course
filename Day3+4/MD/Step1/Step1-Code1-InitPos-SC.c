//Initialise a simple cubic system coordinates
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main()
{
	double cell, *x, *y, *z, lx; // declaring a cubic cell
	int i, n, nc, ix, iy, iz;

	lx = 10.0;
	nc = 4;
	cell = lx/(double)nc;
	n = nc*nc*nc;
	
	//Memory allocation
	x = calloc(n+1, sizeof(double *));
	y = calloc(n+1, sizeof(double *));
	z = calloc(n+1, sizeof(double *));

	//INITIALIZE THE POSITIONS
	n = 1;
	for(iz=1;iz<=nc;iz++)
	for(iy=1;iy<=nc;iy++)
	for(ix=1;ix<=nc;ix++){
	  x[n] = cell*((double)(ix-1));
	  y[n] = cell*((double)(iy-1));
	  z[n] = cell*((double)(iz-1));
	  n = n + 1;
	}

        //SHIFTING COM TO ORIGIN
       	for(i = 1; i < n; i++){
	    	x[i] = x[i] - lx*0.5;
	    	y[i] = y[i] - lx*0.5;
	    	z[i] = z[i] - lx*0.5;
      	}

	printf("%d\n\n",n-1);
	for(i=1;i<n;i++)
	{
		printf("%f\t%f\t%f\n",x[i], y[i], z[i]);
	}
}
