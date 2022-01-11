# warped_discs
## Set up a warped and twisted disc for RADMC-3D simulations
###### Description
**Warped and twisted protoplanetary disc**
*Transition-style disc with cavity separating inner and outer discs*


RADMC-3D [(Dullemond, C. P et al. 2012)](https://ui.adsabs.harvard.edu/abs/2012ascl.soft02015D/abstract)

Rotations performed via quaternion algebra, inspired by [Rosenfeld, K. A et al. 2012](https://ui.adsabs.harvard.edu/abs/2012ApJ...757..129R/abstract)

Accompanying paper: Cummins, D. P et al 2022

###### Files
- problem_setup.c
- star_setup.c
- coord_fcns.h
- warp_fcns.h
- quaternion.h

###### Compilation
Requirements:
- a c compiler, e.g. gcc

Check makefile, set CC to your C compiler (default: gcc)
```
make star
make default
```

###### Running
`./star_setup`

`./problem_setup`