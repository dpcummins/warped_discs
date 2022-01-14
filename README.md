# warped_discs
## Set up a warped and twisted disc for RADMC-3D simulations
### Description
**Warped and twisted protoplanetary disc**

*Transition-style disc with cavity separating inner and outer discs*


RADMC-3D [(Dullemond et al. 2012)](https://ui.adsabs.harvard.edu/abs/2012ascl.soft02015D/abstract)

Rotations performed via quaternion algebra, inspired by [Rosenfeld et al. 2012](https://ui.adsabs.harvard.edu/abs/2012ApJ...757..129R/abstract)

Accompanying paper: Cummins et al 2022

### Files
- problem_setup.c
  - creates the 3D spatial grid and dust density distribution
- star_setup.c
  - creates the stellar properties and wavelength grid
- coord_fcns.h
  - contains useful functions for creating coordinates
- warp_fcns.h
  - contains functions for specifying warp and twist angles
- quaternion.h
  - contains functions for performing quaternion algebra

### Compilation
Requirements:
- a c compiler, e.g. gcc, icc

Check makefile, set CC to your C compiler (default: gcc)

`make star` compiles star_setup.c

`make default` compiles problem_setup.c with default options

`make zt` compiles problem_setup.c with ZEROTEMP option

`make bin` compiles problem_setup.c with BINARY option

`make ztbin` compiles problem_setup.c with ZEROTEMP and BINARY options

Options:
- ZEROTEMP
  - writes dust temperature file as 0 K throughout disc, so that there is no thermal emission from dust
- BINARY
  - writes dust density and temperature files in binary format for speed and efficiency

### Running
`./star_setup`

`./problem_setup`

`./problem_setup rw0 rw1 w0 w1 t0 t1`
