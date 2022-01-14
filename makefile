CC = gcc
DEPS = quaternion.h coord_fcns.h warp_fcns.h
LIBS = -lm

star: star_setup.c $(DEPS)
	$(CC) star_setup.c -o star_setup $(LIBS)

default: problem_setup.c $(DEPS)
	$(CC) problem_setup.c -o problem_setup $(LIBS)

bin: problem_setup.c $(DEPS)
	$(CC) problem_setup.c -o problem_setup $(LIBS) -DBINARY

zt: problem_setup.c $(DEPS)
	$(CC) problem_setup.c -o problem_setup $(LIBS) -DZEROTEMP

ztbin: problem_setup.c $(DEPS)
	$(CC) problem_setup.c -o problem_setup $(LIBS) -DBINARY -DZEROTEMP

.PHONY: clean
clean:
	rm -f problem_setup star_setup amr_grid.inp dust_density.inp dust_density.binp \
	dust_temperature.dat dust_temperature.bdat dustopac.inp wavelength_micron.inp \
	stars.inp radmc3d.inp *.o *~
