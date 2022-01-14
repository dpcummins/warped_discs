double warp_angle(double r, double r0, double r1, double w0, double w1) {
    double w;
    if(r<=r0) {
        w = w0;
    } else if(r>r0 && r<r1) {
        w = (w0-w1)*0.5*(1.0+cos(PI*(r-r0)/(r1-r0))) + w1;
    } else {
        w = w1;
    }
    return(w);
}

double twist_angle(double r, double r0, double r1, double t0, double t1) {
    double t;
    if(r<=r0) {
        t = t0;
    } else if(r>r0 && r<r1) {
        t = (t0-t1)*0.5*(1.0+cos(PI*(r-r0)/(r1-r0))) + t1;
    } else {
        t = t1;
    }
    return(t);
}
