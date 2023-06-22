// Code for performing Molecular Dynamic Simulation - Energy and Force calculations

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
//#include<gsl/gsl_rng.h>
//#include<gsl/gsl_randist.h>

#define eqmdsteps 50000
#define datamdsteps 100000
#define dt 0.002

long int lround(double x);

int main(int argc, char *argv[]){
	//COMMON VARIABLES
	int i, j, num, mdstep, atomdone, atomnotdone;

	//Molecular Dynamics VARIABLES
	double Rho, lx, ly, lz, xij, yij, zij, rsq, mindistsq, mindist;
	double *x, *y, *z, *vx, *vy, *vz, *Fx, *Fy, *Fz, *rmin;
	double Fxmod, Fymod, Fzmod, Fxparch, Fyparch, Fzparch, Fxparlj, Fyparlj, Fzparlj;
	double UljFT, UljST,Ulj, Forcexlj, Forceylj, Forcezlj;
	double Uch, Forcexch, Forceych, Forcezch;
	double T, Trt, r, F, eps;
	double xtmp, ytmp, ztmp, dX, dY, dZ, rc, rcsq, rijsq;
	double xcom, ycom, zcom, vxcom, vycom, vzcom;

	// Variables used while initialization
	int *idx;
	double b0;
	FILE *fpinp;

	//variables used while integration
	double *xold, *yold, *zold, *vxold, *vyold, *vzold;
	double *Fxold, *Fyold, *Fzold, pe, ke; 

	//Random Number Variables
	/*const gsl_rng_type *Q;
	gsl_rng * rgsl;
        Q = gsl_rng_mt19937; //MT RAND. NUM. GENERATOR //Q = gsl_rng_default;
        rgsl = gsl_rng_alloc (Q);
        gsl_rng_set(rgsl, 100.0);
        gsl_rng_env_setup();*/

	//Input Number of particles and Temperature //
	if(argc<1+2){
	   printf("Error!!!\n");
	   printf("Format: %s <No.of particles> <Temperature>\n",argv[0]);
   	   exit(1);
	}
	num = atoi(argv[1]);
	T = atof(argv[2]);

	//DEFINE THE BOX DIMENSION
	lx = 10.0;
	ly = 10.0;
	lz = 10.0;
	mindist = 0.9;
        mindistsq = mindist*mindist;
	
	//POTENTIAL CUT-OFF AND ENERGY
	rc = 2.5; rcsq = rc*rc;
	eps = 1.0;

	//Memory Initialization
	x = calloc(num+1, sizeof(double));
	y = calloc(num+1, sizeof(double));
	z = calloc(num+1, sizeof(double));

	xold = calloc(num+1, sizeof(double));
	yold = calloc(num+1, sizeof(double));
	zold = calloc(num+1, sizeof(double));
	
	vx = calloc(num+1, sizeof(double));
	vy = calloc(num+1, sizeof(double));
	vz = calloc(num+1, sizeof(double));

	vxold = calloc(num+1, sizeof(double));
	vyold = calloc(num+1, sizeof(double));
	vzold = calloc(num+1, sizeof(double));
	
	Fx = calloc(num+1, sizeof(double));
	Fy = calloc(num+1, sizeof(double));
	Fz = calloc(num+1, sizeof(double));

	Fxold = calloc(num+1, sizeof(double));
	Fyold = calloc(num+1, sizeof(double));
	Fzold = calloc(num+1, sizeof(double));

	rmin = calloc(num+1, sizeof(double));
	idx = calloc(num+1, sizeof(int));

	//----------------Initialize the system with n no.of particles-------------------//
	for(i=1; i<=num; i++){
           atomdone = 0; 
	   while( atomdone==0 ){
                xtmp = lx*drand48();
                ytmp = ly*drand48(); 
                ztmp = lz*drand48(); 
                  
		atomnotdone = 0;
                for( j=1; j<=i-1; j++){
                   dX = (xtmp-x[j]) - lx*lround((xtmp-x[j])/lx);
                   dY = (ytmp-y[j]) - ly*lround((ytmp-y[j])/ly);
                   dZ = (ztmp-z[j]) - lz*lround((ztmp-z[j])/lz);
                       
		   rijsq = dX*dX + dY*dY + dZ*dZ;
                       
		   if( rijsq<mindistsq )atomnotdone = 1;
		}
		atomdone = 1-atomnotdone;
                  
		x[i] = xtmp;
                y[i] = ytmp;
                z[i] = ztmp;
           }
	   if(i%10==0) fprintf( stderr, "Atom %d Inserted\n", i );
	}
          
	//SHIFTING COM TO ORIGIN
        for( i=1; i<=num; i++){
           x[i] = x[i] - lx*0.5;
           y[i] = y[i] - ly*0.5;
           z[i] = z[i] - lz*0.5;
	}
	fprintf( stderr, "CONFIGURATION SHIFED TO ORIGIN\n" );

	//Initialize velocities of the system from Gaussian distribution    //
	/*
	Trt = pow(T,0.5);

	vxcom = 0.0;
        vycom = 0.0;
        vzcom = 0.0;

	for(i=1;i<=num;i++){
	   vx[i]=gsl_ran_gaussian(rgsl, Trt);
	   vy[i]=gsl_ran_gaussian(rgsl, Trt);
	   vz[i]=gsl_ran_gaussian(rgsl, Trt);
    	 //  printf("%f\t%f\t%f\n", vx[i],vy[i],vz[i])
	   
	  
	   vxcom = vxcom + vx[i];
	   vycom = vycom + vy[i];
	   vzcom = vzcom + vz[i];
	}
	vxcom = vxcom/num;
     	vycom = vycom/num;
	vzcom = vzcom/num;

	for(i=1;i<=num;i++){	   	
  	   vx[i] = vx[i] - vxcom;
	   vy[i] = vy[i] - vycom;
	   vz[i] = vz[i] - vzcom;
	}*/
	// ------------------------- Initialization done ------------------------- //

	//----------------------ZERO STEP OF MD-------------------------// 
	//COMPUTE THE ENERGY AND TOTAL FORCE ON THE SYSTEM/
	pe = 0.0;
	for(i=1;i<=num;i++){
	    double neigpar = 0;
	    Uch = 0.0;
	    for(j=1; j<=num; j++){
	       if(i!=j){
		  xij = (x[i] - x[j]) - lx*lround((x[i] - x[j])/lx);
		  yij = (y[i] - y[j]) - ly*lround((y[i] - y[j])/ly);
		  zij = (z[i] - z[j]) - lz*lround((z[i] - z[j])/lz);

		  rsq = ((xij*xij) + (yij*yij) + (zij*zij));
		 
		  r = sqrt(rsq);

		  if(rsq <rcsq){ 
		      Uch += 1.0;
		      neigpar += 1;
		  }
	       }
	    }
	    //printf("%f\n", Uch);
	    pe = pe + (Uch/(neigpar*1.0));
	}
	printf("POTENTIAL ENERGY OF THE SYSTEM : %f\n", pe);

	Fxmod = 0.0;
	Fymod = 0.0;
	Fzmod = 0.0;
	for(i=1;i<=num;i++){
	   Fxparlj = 0.0;
 	   Fyparlj = 0.0;
	   Fzparlj = 0.0;
	   double neigpar = 0;
	   Uch = 0.0;
	   for(j=1; j<=num; j++){
	       if(i!=j){
		  xij = (x[i] - x[j]) - lx*lround((x[i] - x[j])/lx);
		  yij = (y[i] - y[j]) - ly*lround((y[i] - y[j])/ly);
		  zij = (z[i] - z[j]) - lz*lround((z[i] - z[j])/lz);

		  rsq = ((xij*xij) + (yij*yij) + (zij*zij));
		 
		  r = sqrt(rsq);

		  if(rsq <rcsq){ 
                    Forcexlj = (48/pow(r,2))*(1/pow(r,12) - (0.5/pow(r,6)))*xij;
		    Forceylj = (48/pow(r,2))*(1/pow(r,12) - (0.5/pow(r,6)))*yij;
		    Forcezlj = (48/pow(r,2))*(1/pow(r,12) - (0.5/pow(r,6)))*zij;

		       
		    Fxparlj = Fxparlj + Forcexlj;
		    Fyparlj = Fyparlj + Forceylj;
		    Fzparlj = Fzparlj + Forcezlj;
		  }else{
		       Forcexlj = 0;
		       Forceylj = 0;
		       Forcezlj = 0;
		  }
	       }
	   }
	   Fx[i] = Fxparlj;
     	   Fy[i] = Fyparlj;
	   Fz[i] = Fzparlj;
	   Fxmod += sqrt(Fx[i]*Fx[i]);
	   Fymod += sqrt(Fy[i]*Fy[i]);
	   Fzmod += sqrt(Fz[i]*Fz[i]);
	}
	printf("TOTAL FORCE ON THE SYSTEM : %f\t%f\t%f\n", Fxmod, Fymod, Fzmod);
}
