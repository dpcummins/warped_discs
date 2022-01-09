#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define PI 3.141592653589793
#define AU 1.49598E13

#include "coord_fcns.h"
#include "quaternion.h"
#include "warp_fcns.h"

int main(int argc, char* argv[]) {

    //// Variable declarations ////
    // Grid variables
    int csys;
    int nr, ntheta, nphi;
    long ntot;
    double r0, r1;
    double theta0, theta1;
    double phi0, phi1;
    double* r_b, * r_c;
    double* theta_b, * theta_c;
    double* phi_b, * phi_c;
    double* rc_grid, * thetac_grid, * phic_grid;
    double* xc_grid, * yc_grid, * zc_grid;

    // Warp and twist variables
    double rw0, rw1;
    double w0, w1;
    double t0, t1;
    int idx0, idx1;
    double* warp_angles, * twist_angles;
    double rot_axis[3];

    // Dust density variables
    double p, q, h0, R0, sigma_d0;
    double* H_d, * sigma_d, * rho_d;

    // Dust temperature
    #if defined(ZEROTEMP) && defined(BINARY)
        double* temp_d;
    #endif

    // Dust variables
    long ndspec;
    char** dspecs;
    int ftype;
    
    // MC variables
    int nphot, nphot_scat, scatmode, countwrite;

    // Misc
    FILE* fid;
    long fmt;
    long precis;
    int i, j, k, l;
    
    //// Make grid ////
    csys = 100;
    nr = 288;
    ntheta = 128;
    nphi = 192;
    ntot = nr*ntheta*nphi;

    // Make boundary coordinates
    r_b = malloc((nr+1)*sizeof(double));
    theta_b = malloc((ntheta+1)*sizeof(double));
    phi_b = malloc((nphi+1)*sizeof(double));

    r0 = 0.5*AU;
    r1 = 150*AU;
    theta0 = 0.0;
    theta1 = PI;
    phi0 = 0.0;
    phi1 = 2*PI;

    logspace(r_b, r0, 13*AU, 128, 0);
    logspace(&(r_b[128]), 13*AU, 103*AU, 64, 0);
    logspace(&(r_b[128+64]), 103*AU, r1, 96+1, 1);
    linspace(theta_b, theta0, theta1, ntheta+1, 1);
    linspace(phi_b, phi0, phi1, nphi+1, 1);

    // Make centre coordinates
    r_c = malloc(nr*sizeof(double));
    theta_c = malloc(ntheta*sizeof(double));
    phi_c = malloc(nphi*sizeof(double));
    for(i=0; i<nr; i++) {
        r_c[i] = 0.5*(r_b[i]+r_b[i+1]);
    }
    for(i=0; i<ntheta; i++) {
        theta_c[i] = 0.5*(theta_b[i]+theta_b[i+1]);
    }
    for(i=0; i<nphi; i++) {
        phi_c[i] = 0.5*(phi_b[i]+phi_b[i+1]);
    }

    // Make grid coordinates
    rc_grid = malloc(ntot*sizeof(double));
    thetac_grid = malloc(ntot*sizeof(double));
    phic_grid = malloc(ntot*sizeof(double));

    zc_grid = malloc(ntot*sizeof(double));

    meshgrid3d(rc_grid, thetac_grid, phic_grid, r_c, theta_c, phi_c, nr, ntheta, nphi);

    //// Warp and twist grid ////
    rw0 = atof(argv[1])*AU;
    rw1 = atof(argv[2])*AU;
    w0 = atof(argv[3])*PI/180;
    w1 = atof(argv[4])*PI/180;
    t0 = atof(argv[5])*PI/180;
    t1 = atof(argv[6])*PI/180;

    idx0 = get_index(r_c, rw0, nr);
    idx1 = get_index(r_c, rw1, nr);
    
    warp_angles = malloc(nr*sizeof(double));
    twist_angles = malloc(nr*sizeof(double));

//    linspace(warp_angles, w0, w0, idx0, 0);
//    linspace(&(warp_angles[idx0]), w1, w1, nr-idx0, 1);

//    linspace(twist_angles, t0, t0, idx0, 0);
//    linspace(&(twist_angles[idx0]), t1, t1, nr-idx0, 1);

    for(i=0; i<ntot; i++) {
        warp_angles[i%nr] = warp_angle(r_c[i%nr], rw0, rw1, w0, w1);
        twist_angles[i%nr] = twist_angle(r_c[i%nr], rw0, rw1, t0, t1);
//        printf("warp_angles[%d] = %f\n", i
        rot_axis[0] = sin(twist_angles[i%nr]);
        rot_axis[1] = cos(twist_angles[i%nr]);
        rot_axis[2] = 0.0;

        quatRot(&(rc_grid[i]), &(thetac_grid[i]), &(phic_grid[i]),
            warp_angles[i%nr], rot_axis);
    }
    free(warp_angles);
    free(twist_angles);

    //// Make dust distribution ////
    p = 1.0;
    q = 1.25;
    h0 = 0.08;
    R0 = 10.0*AU;
    sigma_d0 = 1.0E-1;

    H_d = malloc(ntot*sizeof(double));
    sigma_d = malloc(ntot*sizeof(double));
    rho_d = malloc(ntot*sizeof(double));
    #if defined(ZEROTEMP) && defined(BINARY)
        temp_d = malloc(ntot*sizeof(double));
    #endif

    for(i=0; i<ntot; i++) {
        zc_grid[i] = rc_grid[i]*cos(thetac_grid[i]);
        H_d[i] = h0*R0*pow(rc_grid[i]/R0, q);
        sigma_d[i] = sigma_d0*pow(rc_grid[i]/R0, -p) *
            0.5*(1.0 + erf((rc_grid[i]-1.5*AU)/(1*AU)) +
            erf(-(rc_grid[i]-10*AU)/(1*AU)) +
            erf((rc_grid[i]-110*AU)/(5*AU)));
        rho_d[i] = (1.0/sqrt(2*PI))*(sigma_d[i]/H_d[i]) *
            exp(-zc_grid[i]*zc_grid[i]/(2.0*H_d[i]*H_d[i]));
        #if defined(ZEROTEMP) && defined(BINARY)
            temp_d[i] = 0.0;
        #endif
    }
    free(rc_grid);
    free(thetac_grid);
    free(phic_grid);
    free(H_d);
    free(sigma_d);

    //// Write input files ////
    // Grid file
    fmt = 1;
    fid = fopen("amr_grid.inp", "w");
    fprintf(fid, "%d\n", fmt); // format = 1
    fprintf(fid, "0\n");        // amr grid style
    fprintf(fid, "%d\n", csys); // coordinate system
    fprintf(fid, "0\n");        // grid info
    fprintf(fid, "1 1 1\n");    // dimensions to include
    fprintf(fid, "%d %d %d\n", nr, ntheta, nphi); // grid size
    for(i=0; i<nr+1; i++) {     // radmc3d expects r, theta, phi
        fprintf(fid, "%13.6e\n", r_b[i]);
    }
    for(i=0; i<ntheta+1; i++) {
         fprintf(fid, "%13.6e\n", theta_b[i]);
    }
    for(i=0; i<nphi+1; i++) {
        fprintf(fid, "%13.6e\n", phi_b[i]);
    }
    fclose(fid);
    free(r_b);
    free(theta_b);
    free(phi_b);

    // Density file
    ndspec = 1;
    fmt = 1;
    precis = 8;
    #ifdef BINARY
        fid = fopen("dust_density.binp", "wb");
        fwrite(&fmt, 8, 1, fid);
        fwrite(&precis, 8, 1, fid);
        fwrite(&ntot, 8, 1, fid);
        fwrite(&ndspec, 8, 1, fid);
        fwrite(rho_d, 8, ntot, fid);
        fclose(fid);
    #else
        fid = fopen("dust_density.inp", "w");
        fprintf(fid, "%d\n", fmt);
        fprintf(fid, "%d\n", ntot);
        fprintf(fid, "%d\n", ndspec);
        for(i=0; i<ntot; i++) {
            fprintf(fid, "%13.6e\n", rho_d[i]);
        }
        fclose(fid);
    #endif
    free(rho_d);

    // Temperature file - set temperature zero everywhere for pure scattering
    #ifdef ZEROTEMP
        #ifdef BINARY
            fid = fopen("dust_temperature.bdat", "wb");
            fwrite(&fmt, 8, 1, fid);
            fwrite(&precis, 8, 1, fid);
            fwrite(&ntot, 8, 1, fid);
            fwrite(&ndspec, 8, 1, fid);
            fwrite(temp_d, 8, ntot, fid);
            fclose(fid);
            free(temp_d);
        #else
            fid = fopen("dust_temperature.dat", "w");
            fprintf(fid, "%f\n", fmt);
            fprintf(fid, "%d\n", ntot);
            fprintf(fid, "%d\n", ndspec);
            for(i=0; i<ntot; i++) {
                fprintf(fid, "%f\n", 0.0);
            }
            fclose(fid);
        #endif
    #endif

    // Opacity control file
    dspecs = malloc(ndspec*sizeof(char*));
    dspecs[0] = "pyrmg70_1um";
//    dspecs[1] = "pyrmg70_10um";
    fmt = 2;
    ftype = 10;
    fid = fopen("dustopac.inp", "w");
    fprintf(fid, "%d\t\t\tFormat number of this file\n", fmt);
    fprintf(fid, "%d\t\t\tNumber of dust species\n", ndspec);
    fprintf(fid, "==============================================================\n");
    for(i=0; i<ndspec; i++) {
        fprintf(fid, "%d\t\t\t1:kappa; 10:kapscatmat; -1:opac\n", ftype);
        fprintf(fid, "0\t\t\t0=Thermal grain\n");
        fprintf(fid, "%s\t\t\tExtension of dustkappa_***.inp file\n", dspecs[i]);
        fprintf(fid, "----------------------------------------------------------\n");
    }
    fclose(fid);
    free(dspecs);

    // radmc3d control file
    nphot = 1E6;
    nphot_scat = nphot;
    scatmode = 5;
    countwrite = 1E5;
    fid = fopen("radmc3d.inp", "w");
    fprintf(fid, "nphot = %d\n", nphot);
    fprintf(fid, "nphot_scat = %d\n", nphot_scat);
    fprintf(fid, "scattering_mode_max = %d\n", scatmode);
    fprintf(fid, "countwrite = %d\n", countwrite);
    fprintf(fid, "iranfreqmode = %d\n", 1);
    fclose(fid);

    return(0);
}
