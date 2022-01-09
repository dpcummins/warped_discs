CC = gcc
DEPS = quaternion.h cood_fcns.h warp_fcns.h
LIBS = -lm
OBJS = problem_setup.o star_setup.o

bin: $(OBJS)
	$(CC) $(OBJS) -o problem_setup -DBINARY

zt: $(OBJS)
	$(CC) $(OBJS) -o problem_setup -DZEROTEMP

ztbin: $(OBJS)
	$(CC) $(OBJS) -o problem_setup -DBINARY -DZEROTEMP

.PHONY: clean
clean:
	rm problem_setup star_setup amr_grid.inp dust_density.inp dust_density.binp \
	dust_temperature.dat dust_temperature.bdat dustopac.inp wavelength_micron.inp \
	stars.inp radmc3d.inp *.o *~
