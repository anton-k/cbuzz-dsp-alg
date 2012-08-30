vpath %.c src
vpath %.o obj
vpath %.out obj

objdir		= obj
srcdir		= src
cflags      = -Wall -g -ggdb

# trick to add path to all files
_objects = dsp-alg-types.o  dsp-alg-num.o time.o \
	dsp-alg-phasor.o \
	dsp-alg-filter.o \
	dsp-alg-envelope.o 
#_objects 	= dsp-alg-types.o time.o dsp-alg-io.o \
#	dsp-alg-num.o dsp-alg-math.o \
#	dsp-alg-envelope.o dsp-alg-phasor.o dsp-alg-table.o \
#	dsp-alg-filter.o dsp-alg-buffer.o \
#	dsp-alg-sndfile.o
objects 	= $(patsubst %,$(objdir)/%,$(_objects))

includes	= -I include
libs		= -lm -lsndfile -lcalg

cc			= gcc

$(objdir)/%.o: $(srcdir)/%.c
	$(cc) $(cflags) -c $< -o $@ $(includes) $(libs)  

all: test.o

test.o: $(objects)
	$(cc) $(cflags) $^ test.c $(includes) $(libs) 
	mv a.out obj

clean:
	rm -f $(objects); rm -f $(objdir)/a.out


a.out: test.o

run: a.out test.c
	@echo ----------------------------------------------
	@echo 
	@echo 
	obj/a.out

# plot output and supress warnings from GTK (2>/dev/null)
plot: a.out test.c
	@obj/a.out
	@gnuplot -e "plot \"fig\" with lines lc 3; pause -1" 2>/dev/null
	

debug: a.out
	@echo ----------------------------------------------
	@echo 
	@echo
	gdb obj/a.out	
	


