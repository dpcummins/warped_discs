void linspace(double* x, double x0, double x1, int nx, int endpoint) {
    int i;
    double dx;
    if(endpoint) {
        dx = (x1-x0)/(nx-1);
    } else {
        dx = (x1-x0)/nx;
    }
    x[0] = x0;
    for(i=1; i<nx; i++) {
        x[i] = x0 + i*dx;
    }
}

void logspace(double* x, double x0, double x1, int nx, int endpoint) {
    int i;
    double dx;
    if(endpoint) {
        dx = (log(x1)-log(x0))/(nx-1);
    } else {
        dx = (log(x1)-log(x0))/nx;
    }
    x[0] = x0;
    for(i=1; i<nx; i++) {
        x[i] = exp(log(x0) + i*dx);
    }
}

void meshgrid2d(double* xgrid, double* ygrid, double* x, double* y,
                int nx, int ny) {
    int i, j, l;
    for(j=0; j<ny; j++) {
        for(i=0; i<nx; i++) {
            l = j*nx + i;
            xgrid[l] = x[i];
            ygrid[l] = y[j];
        }
    }
}

void meshgrid3d(double* xgrid, double* ygrid, double* zgrid,
                double* x, double* y, double* z, int nx, int ny, int nz) {
    int i, j, k, l;
    for(k=0; k<nz; k++) {
        for(j=0; j<ny; j++) {
            for(i=0; i<nx; i++) {
                l = k*ny*nx + j*nx + i;    // column-major indexing
                xgrid[l] = x[i];
                ygrid[l] = y[j];
                zgrid[l] = z[k];
            }
        }
    }
}

int get_index(double* x, double x0, int nx) {
    int i;
    for(i=0; i<nx-1; i++) {
        if(x[i]>=x0) break;
    }
    return(i);
}
