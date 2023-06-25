// Code for performing Molecular Dynamic Simulation

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define eqmdsteps 10000
#define datamdsteps 10000
#define dt 0.002

long int lround(double x);

int mdstep, atomdone, atomnotdone;
double rho, lx, ly, lz, xij, yij, zij, rsq, mindistsq, mindist;
double xtmp, ytmp, ztmp;
double *x, *y, *z, *vx, *vy, *vz, *Fx, *Fy, *Fz;
double Fxpar, Fypar, Fzpar;
double UljFT, UljST, Ulj, Forcexlj, Forceylj, Forcezlj;
double T, Trt, r;

double vxcom, vycom, vzcom;
double rc, rcsq;
// Variables used while initialization

int i, j, num, n, choice;
FILE *fpconf, *fpout, *fpout2;

// variables used while integration
double *xold, *yold, *zold, *vxold, *vyold, *vzold;
double *Fxold, *Fyold, *Fzold, pe, ke;

typedef struct neighbour_node {
  double x_neighbour;
  double y_neighbour;
  double z_neighbour;
  struct neighbour_node *next;
} NN;

typedef struct neightbour_list {
  NN *neighbours;
} NLIST;

void add_neighbour(NLIST *N_head, int i, int j, int fresh) {
  if (N_head[i].neighbours == NULL || fresh == 1) {
    N_head[i].neighbours = (NN *)malloc(sizeof(NN));
    N_head[i].neighbours->next = NULL;
  }
  NN *temp = N_head[i].neighbours;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = (NN *)malloc(sizeof(NN));
  temp->next->next = NULL;
  temp->next->x_neighbour = x[j];
  temp->next->y_neighbour = y[j];
  temp->next->z_neighbour = z[j];
}

void update_neighbour_list(NLIST *N_head) {
  double rnl = 3.0;
  double rnlsq = rnl * rnl;
  double *x_temp = x;
  double *y_temp = y;
  double *z_temp = z;
  double xij_temp = xij;
  double yij_temp = yij;
  double zij_temp = zij;

  int fresh = 1;
  for (int i = 1; i <= num; i++) {
    for (int j = 1; j <= num; j++) {
      if (i != j) {
        xij_temp =
            (x_temp[i] - x_temp[j]) - lx * lround((x_temp[i] - x_temp[j]) / lx);
        yij_temp =
            (y_temp[i] - y_temp[j]) - lx * lround((y_temp[i] - y_temp[j]) / lx);
        zij_temp =
            (z_temp[i] - z_temp[j]) - lx * lround((z_temp[i] - z_temp[j]) / lx);
        rsq = (xij_temp * xij_temp) + (yij_temp * yij_temp) +
              (zij_temp * zij_temp);
        if (rsq < rnlsq) {
          add_neighbour(N_head, i, j, fresh);
          fresh = 0;
        } else {
          continue;
        }
      }
    }
    fresh = 1;
  }
}

int main(int argc, char *argv[]) {
  // int mdstep, atomdone, atomnotdone;
  // double rho, lx, ly, lz, xij, yij, zij, rsq, mindistsq, mindist;
  // double xtmp, ytmp, ztmp;
  // double *x, *y, *z, *vx, *vy, *vz, *Fx, *Fy, *Fz;
  // double Fxpar, Fypar, Fzpar;
  // double UljFT, UljST, Ulj, Forcexlj, Forceylj, Forcezlj;
  // double T, Trt, r;
  //
  // double vxcom, vycom, vzcom;
  // double rc, rcsq;
  // // Variables used while initialization
  //
  // int i, j, num, n, choice;
  // FILE *fpconf, *fpout, *fpout2;
  //
  // // variables used while integration
  // double *xold, *yold, *zold, *vxold, *vyold, *vzold;
  // double *Fxold, *Fyold, *Fzold, pe, ke;
  const gsl_rng_type *P;
  gsl_rng *m;

  gsl_rng_env_setup();

  P = gsl_rng_default;
  m = gsl_rng_alloc(P);

  // Input Number of particles and Temperature //
  if (argc < 1 + 3) {
    printf("Error!!!\n");
    printf("Format: %s <No.of particles> <Temperature> <Rho>\n", argv[0]);
    exit(1);
  }

  num = atoi(argv[1]);
  T = atof(argv[2]);
  rho = atof(argv[3]);
  lx = pow((num * 1.0 / rho), 1.0 / 3.0);
  ly = lx;
  lz = lx;
  // printf("%f\t%f\t%f\n", lx, ly, lz);

  // Variables for the neighbour list
  NLIST N_head[num];
  for (int i = 1; i <= num; i++) {
    N_head[i].neighbours = NULL;
  }

  x = calloc(num + 1, sizeof(double));
  y = calloc(num + 1, sizeof(double));
  z = calloc(num + 1, sizeof(double));

  xold = calloc(num + 1, sizeof(double));
  yold = calloc(num + 1, sizeof(double));
  zold = calloc(num + 1, sizeof(double));

  vx = calloc(num + 1, sizeof(double));
  vy = calloc(num + 1, sizeof(double));
  vz = calloc(num + 1, sizeof(double));

  vxold = calloc(num + 1, sizeof(double));
  vyold = calloc(num + 1, sizeof(double));
  vzold = calloc(num + 1, sizeof(double));

  Fx = calloc(num + 1, sizeof(double));
  Fy = calloc(num + 1, sizeof(double));
  Fz = calloc(num + 1, sizeof(double));

  Fxold = calloc(num + 1, sizeof(double));
  Fyold = calloc(num + 1, sizeof(double));
  Fzold = calloc(num + 1, sizeof(double));

  if (choice == 1 && fpconf == NULL) {
    printf("File not opened\n");
    exit(1);
  }
  fpout = fopen("Configuration-Raw.xyz", "w");
  fpout2 = fopen("Configuration-PBC.xyz", "w");

  rc = 2.5;
  double rnl = 3.0;
  rcsq = rc * rc;
  double rnlsq = rnl * rnl;

  //**************** Initialize the system with Num_par no.of
  // particles******************* //

  if (choice == 1) {
    fscanf(fpconf, "%d\n\n", &n);
    if (n != num) {
      printf("ERROR!! Number of particles mismatch\n");
      exit(1);
    }
    for (i = 1; i <= num; i++) {
      fscanf(fpconf, "%lf\t%lf\t%lf\n", &xtmp, &ytmp, &ztmp);
      x[i] = xtmp;
      y[i] = ytmp;
      z[i] = ztmp;
    }
    fclose(fpconf);
  } else {
    mindist = 1.1;
    mindistsq = mindist * mindist;
    for (i = 1; i <= num; i++) {
      atomdone = 0;
      while (atomdone == 0) {
        xtmp = lx * (drand48() - 0.5);
        ytmp = ly * (drand48() - 0.5);
        ztmp = lz * (drand48() - 0.5);

        atomnotdone = 0;
        for (j = 1; j <= i - 1; j++) {
          xij = (xtmp - x[j]) - lx * lround((xtmp - x[j]) / lx);
          yij = (ytmp - y[j]) - ly * lround((ytmp - y[j]) / ly);
          zij = (ztmp - z[j]) - lz * lround((ztmp - z[j]) / lz);

          rsq = xij * xij + yij * yij + zij * zij;
          if (rsq < mindistsq)
            atomnotdone = 1;
        }
        atomdone = 1 - atomnotdone;
        x[i] = xtmp;
        y[i] = ytmp;
        z[i] = ztmp;
      }
      // if(i%10==0) fprintf( stderr, "Atom %d Inserted\n", i );
    }
  }

  update_neighbour_list(N_head);

  // Initialize velocities of the system from Gaussian distribution    //
  Trt = pow(T, 1.0 / 2.0);

  vxcom = 0.0;
  vycom = 0.0;
  vzcom = 0.0;

  for (i = 1; i <= num; i++) {
    vx[i] = gsl_ran_gaussian(m, Trt);
    vy[i] = gsl_ran_gaussian(m, Trt);
    vz[i] = gsl_ran_gaussian(m, Trt);

    vxcom = vxcom + vx[i];
    vycom = vycom + vy[i];
    vzcom = vzcom + vz[i];
  }
  vxcom = vxcom / num;
  vycom = vycom / num;
  vzcom = vzcom / num;

  for (i = 1; i <= num; i++) {
    vx[i] = vx[i] - vxcom;
    vy[i] = vy[i] - vycom;
    vz[i] = vz[i] - vzcom;
  }
  // ************************* Initialization done
  // ********************************************* //
  for (i = 1; i <= num; i++) {
    // SAVING TO OLD X,Y,Z
    xold[i] = x[i];
    yold[i] = y[i];
    zold[i] = z[i];

    // SAVING TO OLD VX,VY,VZ
    vxold[i] = vx[i];
    vyold[i] = vy[i];
    vzold[i] = vz[i];
  }

  // //--------------- Start MD loop --------------------//
  //
  // // printf("#mdstep\t TE \t PE \t KE\n");
  // for (mdstep = 1; mdstep <= eqmdsteps; mdstep++) {
  //
  //   // EACH STEP SUM OVER THE PARTICLES AND ITS NEIGBOURS AND
  //   // FIND THE FORCE ON EACH PARTICLE AND ENERGY OF THE SYSTEM
  //   // AFTER FINDING FORCE FIND NEW POSITION AND VELOCITY
  //   for (i = 1; i <= num; i++) {
  //     Fxpar = 0.0;
  //     Fypar = 0.0;
  //     Fzpar = 0.0;
  //     for (j = 1; j <= num; j++) {
  //       if (i != j) {
  //         xij = (x[i] - x[j]) - lx * lround((x[i] - x[j]) / lx);
  //         yij = (y[i] - y[j]) - lx * lround((y[i] - y[j]) / lx);
  //         zij = (z[i] - z[j]) - lx * lround((z[i] - z[j]) / lx);
  //
  //         rsq = (xij * xij) + (yij * yij) + (zij * zij);
  //
  //         // **** Force and energy calculation ****** //
  //         r = sqrt(rsq);
  //
  //         if (rsq <= rcsq) {
  //           // Force corresponding to LJ potential //
  //           Forcexlj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) *
  //           xij; Forceylj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0)))
  //           * yij; Forcezlj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 /
  //           pow(r, 8.0))) * zij;
  //
  //           //	printf("%f\t%f\t%f\t%f\n", r,Forcexlj, Forceylj,
  //           Forcezlj);
  //
  //           Fxpar = Fxpar + Forcexlj;
  //           Fypar = Fypar + Forceylj;
  //           Fzpar = Fzpar + Forcezlj;
  //
  //         } else {
  //           Forcexlj = 0;
  //           Forceylj = 0;
  //           Forcezlj = 0;
  //         }
  //       }
  //     }
  //     Fxold[i] = Fxpar;
  //     Fyold[i] = Fypar;
  //     Fzold[i] = Fzpar;
  //   }

  //--------------- Start MD loop --------------------//

  // printf("#mdstep\t TE \t PE \t KE\n");
  for (mdstep = 1; mdstep <= eqmdsteps; mdstep++) {

    // EACH STEP SUM OVER THE PARTICLES AND ITS NEIGBOURS AND
    // FIND THE FORCE ON EACH PARTICLE AND ENERGY OF THE SYSTEM
    // AFTER FINDING FORCE FIND NEW POSITION AND VELOCITY
    if (mdstep % 20 == 0) {
      update_neighbour_list(N_head);
    }
    for (i = 1; i <= num; i++) {
      Fxpar = 0.0;
      Fypar = 0.0;
      Fzpar = 0.0;
      NN *j_point = N_head[i].neighbours;
      for (; j_point != NULL;) {
        // if (i != j) {
        xij = (x[i] - (j_point->x_neighbour)) -
              lx * lround((x[i] - (j_point->x_neighbour)) / lx);
        yij = (y[i] - (j_point->y_neighbour)) -
              lx * lround((y[i] - (j_point->y_neighbour)) / lx);
        zij = (z[i] - (j_point->z_neighbour)) -
              lx * lround((z[i] - (j_point->z_neighbour)) / lx);
        rsq = (xij * xij) + (yij * yij) + (zij * zij);

        // **** Force and energy calculation ****** //
        r = sqrt(rsq);

        if (rsq <= rcsq) {
          // Force corresponding to LJ potential //
          Forcexlj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) * xij;
          Forceylj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) * yij;
          Forcezlj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) * zij;

          //	printf("%f\t%f\t%f\t%f\n", r,Forcexlj, Forceylj, Forcezlj);

          Fxpar = Fxpar + Forcexlj;
          Fypar = Fypar + Forceylj;
          Fzpar = Fzpar + Forcezlj;

        } else {
          Forcexlj = 0;
          Forceylj = 0;
          Forcezlj = 0;
        }
        // }
        j_point = j_point->next;
      }
      Fxold[i] = Fxpar;
      Fyold[i] = Fypar;
      Fzold[i] = Fzpar;
    }

    // *************** Energy and Force calculation done
    // ************************ //
    // ************* Integrating equation of motion (Velocity verlet) **********
    // //

    // FIRST FIND THE NEW POSITION
    for (i = 1; i <= num; i++) {
      x[i] = xold[i] + vxold[i] * dt + (Fxold[i] / 2.0) * dt * dt;
      y[i] = yold[i] + vyold[i] * dt + (Fyold[i] / 2.0) * dt * dt;
      z[i] = zold[i] + vzold[i] * dt + (Fzold[i] / 2.0) * dt * dt;

      // SET THE NEW TO OLD FOR NEXT LOOP
      xold[i] = x[i];
      yold[i] = y[i];
      zold[i] = z[i];
    }

    // NOW FIND THE FORCE FROM THE ABOVE NEW POSITION
    for (i = 1; i <= num; i++) {
      // SUM OVER NEIGHBOUR IS DONE SO MAKE IT ZERO
      Fxpar = 0.0;
      Fypar = 0.0;
      Fzpar = 0.0;
      NN *j_point = N_head[i].neighbours;
      for (; j_point != NULL;) {
        // if (i != j) {
        // BETTER TO BUILD A NEIGHBOURLIST THAN GOING THROUGH ALL PARTICLES
        xij = (x[i] - j_point->x_neighbour) -
              lx * lround((x[i] - j_point->x_neighbour) / lx);
        yij = (y[i] - j_point->y_neighbour) -
              lx * lround((y[i] - j_point->y_neighbour) / lx);
        zij = (z[i] - j_point->z_neighbour) -
              lx * lround((z[i] - j_point->z_neighbour) / lx);

        rsq = (xij * xij) + (yij * yij) + (zij * zij);

        // **** Force and energy calculation ****** //
        r = sqrt(rsq);
        if (rsq <= rcsq) {

          // Force corresponding to Repulsive LJ potential //
          Forcexlj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) * xij;
          Forceylj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) * yij;
          Forcezlj = 4 * ((12.0 / pow(r, 14.0)) - (6.0 / pow(r, 8.0))) * zij;

          Fxpar = Fxpar + Forcexlj;
          Fypar = Fypar + Forceylj;
          Fzpar = Fzpar + Forcezlj;

        } else {
          Forcexlj = 0.0;
          Forceylj = 0.0;
          Forcezlj = 0.0;
        }
        // }
        j_point = j_point->next;
      }
      // NEW SET OF FORCES - USED TO CALCULATE THE NEW VELOCITIES
      Fx[i] = Fxpar;
      Fy[i] = Fypar;
      Fz[i] = Fzpar;
    }

    // NOW THE NEW FORCES ARE FOUND... GO ON TO CALCULATE NEW VELOCITIES

    for (i = 1; i <= num; i++) {
      vx[i] = vxold[i] + ((Fxold[i] + Fx[i]) / 2.0) * dt;
      vy[i] = vyold[i] + ((Fyold[i] + Fy[i]) / 2.0) * dt;
      vz[i] = vzold[i] + ((Fzold[i] + Fz[i]) / 2.0) * dt;
      // printf("%f\t%f\t%f\n", vx[i], vy[i], vz[i]);

      // SET THE NEW TO OLD FOR NEXT LOOP
      vxold[i] = vx[i];
      vyold[i] = vy[i];
      vzold[i] = vz[i];
    }

    // THERMO OUTPUT
    /*if(mdstep%10 == 0){
      ke = 0.0;
      pe = 0.0;
      for(i=1;i<=num;i++){
         for(j=1;j<=num;j++){
            if(i!=j){
              xij = (x[i] - x[j]) - lx*lround((x[i]-x[j])/lx);
              yij = (y[i] - y[j]) - ly*lround((y[i]-y[j])/ly);
              zij = (z[i] - z[j]) - lz*lround((z[i]-z[j])/lz);

              rsq = (xij*xij)+(yij*yij)+(zij*zij);
               r = sqrt(rsq);
               //printf("%f\n", r);
                //i) LJ potential Calculation  //
                if(rsq <= rcsq){
                  UljFT = 1.0/pow(r,12.0);
                  UljST = 1.0/pow(r,6.0);
                  Ulj  = 4*(UljFT - UljST);
                 //  printf("%f\t%f\n", r, Ulj);
                 }
                 else Ulj = 0.0;
                pe = pe + Ulj ;
              }
         }

         ke = ke + 0.5*(vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
         }

         pe = pe / 2.0;

      printf("%d\t%f\t%f\t%f\n", mdstep, pe+ke, pe, ke);
    }*/

    // DUMP THE CONFIGURATIONS
    if (mdstep % 100 == 0) {
      fprintf(fpout, "%d\n\n", num);
      for (i = 1; i <= num; i++) {
        fprintf(fpout, "%f\t%f\t%f\t%f\t%f\t%f\n", x[i], y[i], z[i], vx[i],
                vy[i], vz[i]);
      }

      fprintf(fpout2, "%d\n\n", num);
      for (i = 1; i <= num; i++) {
        fprintf(fpout2, "%f\t%f\t%f\t%f\t%f\t%f\n",
                x[i] - lx * lround(x[i] / lx), y[i] - ly * lround(y[i] / ly),
                z[i] - lz * lround(z[i] / lz), vx[i], vy[i], vz[i]);
      }
    }
  }
  fclose(fpout);
  fclose(fpout2);
}
