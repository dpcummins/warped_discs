CC = gcc
DEPS = quaternion.h coord_fcns.h warp_fcns.h
LIBS = -lm

star: star_setup.o $(DEPS)
	$(CC) -o star_setup star_setup.o $(LIBS)

bin: problem_setup.o $(DEPS)
	$(CC) -o problem_setup problem_setup.o $(LIBS) -DBINARY

zt: problem_setup.o $(DEPS)
	$(CC) -o problem_setup problem_setup.o $(LIBS) -DZEROTEMP

ztbin: problem_setup.o $(DEPS)
	$(CC) -o problem_setup problem_setup.o $(LIBS) -DBINARY -DZEROTEMP

.PHONY: clean
clean:
	rm -f problem_setup star_setup amr_grid.inp dust_density.inp dust_density.binp \
	dust_temperature.dat dust_temperature.bdat dustopac.inp wavelength_micron.inp \
	stars.inp radmc3d.inp *.o *~
