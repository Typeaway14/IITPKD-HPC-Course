#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
     
int main (void)
{
	int i, n;
	double T, sigma=sqrt(1.5);
      	const gsl_rng_type * Q;
	//Random Number Related
	gsl_rng * r;
	Q = gsl_rng_mt19937; //MT RAND. NUM. GENERATOR //Q = gsl_rng_default;
       	r = gsl_rng_alloc (Q);
      	gsl_rng_set(r, 100.0);
	gsl_rng_env_setup();

	//Initialize constant
	n = 64;
	double vx[n],vy[n],vz[n];
	//Initialize velocities
	for(i=1; i<=n; i++){
           vx[i] = gsl_ran_gaussian(r, sigma) + 0.0; //GAUSSIAN WITH SD sigma AND MEAN 0.0
           vy[i]= gsl_ran_gaussian(r, sigma) + 0.0; //GAUSSIAN WITH SD sigma AND MEAN 0.0
           vz[i] = gsl_ran_gaussian(r, sigma) + 0.0; //GAUSSIAN WITH SD sigma AND MEAN 0.0
     	   printf("%f\t", vx[i]);
     	   printf("%f\t", vy[i]);
     	   printf("%f\n", vz[i]);
    	}
  
	gsl_rng_free (r);
  
	return 0;
}
