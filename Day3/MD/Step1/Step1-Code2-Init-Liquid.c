#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main()
{
	int i, j, n, atomdone, atomnotdone;
	double *x, *y, *z, lx, mindist;
	double xtmp, ytmp, ztmp, mindistsq;
	double dX, dY, dZ, rijsq;
        
	//Random Number Related
	srand48(100);

	//Initialize constant
	lx = 10.0;
	n = 64;
	mindist = 0.9;
        mindistsq = mindist*mindist;
          
	//Memory allocation
	x = calloc(n+1, sizeof(double *));
	y = calloc(n+1, sizeof(double *));
	z = calloc(n+1, sizeof(double *));
	
	for(i=1; i<=n; i++){
           atomdone = 0; 
	   while( atomdone==0 ){
                xtmp = lx*drand48();
                ytmp = lx*drand48(); 
                ztmp = lx*drand48(); 
                  
		atomnotdone = 0;
                for( j=1; j<=i-1; j++){
                   dX = (xtmp-x[j]) - lx*lround((xtmp-x[j])/lx);
                   dY = (ytmp-y[j]) - lx*lround((ytmp-y[j])/lx);
                   dZ = (ztmp-z[j]) - lx*lround((ztmp-z[j])/lx);
                       
		   rijsq = dX*dX + dY*dY + dZ*dZ;
		}
		atomdone = 1-atomnotdone;
                  
		x[i] = xtmp;
                y[i] = ytmp;
                z[i] = ztmp;
             }
             fprintf( stderr, "Atom %d Inserted\n", i );
          }

          //SHIFTING COM TO ORIGIN
          for( i=1; i<=n; i++){
             x[i] = x[i] - lx*0.5;
             y[i] = y[i] - lx*0.5;
             z[i] = z[i] - lx*0.5;
          }
	  fprintf( stderr, "CONFIGURATION SHIFED TO ORIGIN\n" );

	  printf("%d\n\n",n);
  	  for(i=1;i<=n;i++){
	     printf("%f\t%f\t%f\n",x[i], y[i], z[i]);
	  }
        
}
