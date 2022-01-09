#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "coord_fcns.h"

#define MS 1.9891E33
#define RS 6.96E10

int main(int argc, char* argv[]) {

    //// Variable declarations ////
    // Wavelength variables
    int nl12, nl23, nl34;
    double lam1, lam2, lam3, lam4;
    double* lam12, * lam23, * lam34;

    // Stellar variables
    double mstar, rstar, tstar;
    double pstar[3];

    // Misc
    FILE* fid;
    int i;
    int fmt;

    //// Write input files ////
    // Wavelength file
    lam1 = 0.1;
    lam2 = 7.0;
    lam3 = 25.0;
    lam4 = 1.0E4;
    nl12 = 20;
    nl23 = 100;
    nl34 = 30;
    lam12 = malloc(nl12*sizeof(double));
    lam23 = malloc(nl23*sizeof(double));
    lam34 = malloc(nl34*sizeof(double));
    logspace(lam12, lam1, lam2, nl12, 0);
    logspace(lam23, lam2, lam3, nl23, 0);
    logspace(lam34, lam3, lam4, nl34, 1);

    fid = fopen("wavelength_micron.inp", "w");
    fprintf(fid, "%d\n", nl12+nl23+nl34);
    for(i=0; i<nl12; i++) {
        fprintf(fid, "%13.6e\n", lam12[i]);
    }
    for(i=0; i<nl23; i++) {
        fprintf(fid, "%13.6e\n", lam23[i]);
    }
    for(i=0; i<nl34; i++) {
        fprintf(fid, "%13.6e\n", lam34[i]);
    }
    fclose(fid);

    // Star file
    mstar = 2.0*MS;
    rstar = 3.8*RS;
    tstar = 6250;
    pstar[0] = 0.0;
    pstar[1] = 0.0;
    pstar[2] = 0.0;
    fmt = 2; // 1: frequencies; 2: wavelengths
    fid = fopen("stars.inp", "w");
    fprintf(fid, "%d\n", fmt);
    fprintf(fid, "1 %d\n\n", nl12+nl23+nl34);
    fprintf(fid, "%13.6e %13.6e %13.6e %13.6e %13.6e\n\n",
            rstar, mstar, pstar[0], pstar[1], pstar[2]);
    for(i=0; i<nl12; i++) {
        fprintf(fid, "%13.6e\n", lam12[i]);
    }
    for(i=0; i<nl23; i++) {
        fprintf(fid, "%13.6e\n", lam23[i]);
    }
    for(i=0; i<nl34; i++) {
        fprintf(fid, "%13.6e\n", lam34[i]);
    }
    fprintf(fid, "\n%13.6e\n", -tstar);
    fclose(fid);
    free(lam12);
    free(lam23);
    free(lam34);
}
