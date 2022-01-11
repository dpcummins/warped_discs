#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "coord_fcns.h"

#define MS 1.9891E33
#define RS 6.96E10

int main(int argc, char* argv[]) {

    //// Variable declarations ////
    // Wavelength variables
    int nl01, nl12, nl23;
    double lam0, lam1, lam2, lam3;
    double* lam01, * lam12, * lam23;

    // Stellar variables
    double mstar, rstar, tstar;
    double pstar[3];

    // Misc
    FILE* fid;
    int i;
    int fmt;

    //// Write input files ////
    // Wavelength file
    lam0 = 0.1;  // all in microns
    lam1 = 7.0;
    lam2 = 25.0;
    lam3 = 1.0E4;
    nl01 = 20;
    nl12 = 100;
    nl23 = 30;
    lam01 = malloc(nl01*sizeof(double));
    lam12 = malloc(nl12*sizeof(double));
    lam23 = malloc(nl23*sizeof(double));
    logspace(lam01, lam0, lam1, nl01, 0);
    logspace(lam12, lam1, lam2, nl12, 0);
    logspace(lam23, lam2, lam3, nl23, 1);

    fid = fopen("wavelength_micron.inp", "w");
    fprintf(fid, "%d\n", nl01+nl12+nl23);
    for(i=0; i<nl01; i++) {
        fprintf(fid, "%13.6e\n", lam01[i]);
    }
    for(i=0; i<nl12; i++) {
        fprintf(fid, "%13.6e\n", lam12[i]);
    }
    for(i=0; i<nl23; i++) {
        fprintf(fid, "%13.6e\n", lam23[i]);
    }
    fclose(fid);

    // Star file
    mstar = 2.0*MS;    // stellar mass
    rstar = 3.8*RS;    // stellar radius
    tstar = 6250;      // stellar temperature
    pstar[0] = 0.0;    // stellar position
    pstar[1] = 0.0;
    pstar[2] = 0.0;
    fmt = 2; // 1: frequencies; 2: wavelengths
    fid = fopen("stars.inp", "w");
    fprintf(fid, "%d\n", fmt);
    fprintf(fid, "1 %d\n\n", nl01+nl12+nl23);
    fprintf(fid, "%13.6e %13.6e %13.6e %13.6e %13.6e\n\n",
            rstar, mstar, pstar[0], pstar[1], pstar[2]);
    for(i=0; i<nl01; i++) {
        fprintf(fid, "%13.6e\n", lam01[i]);
    }
    for(i=0; i<nl12; i++) {
        fprintf(fid, "%13.6e\n", lam12[i]);
    }
    for(i=0; i<nl23; i++) {
        fprintf(fid, "%13.6e\n", lam23[i]);
    }
    fprintf(fid, "\n%13.6e\n", -tstar);
    fclose(fid);
    free(lam01);
    free(lam12);
    free(lam23);
}
