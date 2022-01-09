// quaternion multiplication
void qMult(double* p, double* q, double* qprod) {

    // p = [r, U] = [r, u_x, u_y, u_z]
    // q = [s, V]

    double r, s;
    double u[3], v[3];
    
    r = p[0];
    u[0] = p[1];
    u[1] = p[2];
    u[2] = p[3];

    s = q[0];
    v[0] = q[1];
    v[1] = q[2];
    v[2] = q[3];

    qprod[0] = r*s - u[0]*v[0] - u[1]*v[1] - u[2]*v[2];
    qprod[1] = r*v[0] + s*u[0] + u[1]*v[2] - u[2]*v[1];
    qprod[2] = r*v[1] + s*u[1] - u[0]*v[2] + u[2]*v[0];
    qprod[3] = r*v[2] + s*u[2] + u[0]*v[1] - u[1]*v[0];
}

// quaternion conjugation
void qConj(double* q, double* qstar) {

    // qstar = conjugate([s, V]) = [s, -V]

    qstar[0] = q[0];
    qstar[1] = -q[1];
    qstar[2] = -q[2];
    qstar[3] = -q[3];
}

// rotation via quaternion multiplication
void quatRot(double* r, double* theta, double* phi, double angle, double* axis) {

    // q  = [s, V] = [cos(angle/2), axis*sin(theta/2)]
    // v0 = [0, V] = [0, x, y, z]    // V = vector to rotate
    // w0 = q*v0*qstar = [0, W]      // W = rotated vector
    
    double x, y, z;
    double q[4], qstar[4], v0[4], w0[4], temp[4];
    int i;

    x = (*r)*sin(*theta)*cos(*phi);
    y = (*r)*sin(*theta)*sin(*phi);
    z = (*r)*cos(*theta);

    q[0] = cos(0.5*angle);
    q[1] = axis[0]*sin(0.5*angle);
    q[2] = axis[1]*sin(0.5*angle);
    q[3] = axis[2]*sin(0.5*angle);

    v0[0] = 0;
    v0[1] = x;
    v0[2] = y;
    v0[3] = z;

    w0[0] = 0;
        
    qConj(q, qstar);
    qMult(v0, qstar, temp);
    qMult(q, temp, w0);

    x = w0[1];
    y = w0[2];
    z = w0[3];

    *r = sqrt(x*x + y*y + z*z);
    *theta = acos(z/(*r));
    *phi = atan2(y, x);
}
