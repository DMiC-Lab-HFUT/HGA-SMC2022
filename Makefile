CC=gcc
CFLAGS=-O3 -I. -Wall -DCEC2010
#CFLAGS=-O3 -I. -DSAMPLE
PROBLEMS=\
	problems/sample-def problems/sample-f problems/sample-gh \
		problems/sample-nabla \
	problems/cec2010-def
CONSTRAINTS=\
	constraints.o epsilon/compare.o epsilon/epsilon.o
GRADIENT=\
	nabla.o gradient/gradient.o gradient/operation.o gradient/gsolve-gsl.o
OPTIMIZE=init.o eval.o new.o utils.o version.o params.o problem.o
TOOLS=lib/rand.o
EXTERNAL=CEC2010/cec2010.o CEC2010/dll.o \
		CEC2010/fcnsuite.o #CEC2010/fcnsuite.dll
# CEC2010 for WINDOWS
#CLIBS=-lm -lgsl
# CEC2010 for LINUX
CLIBS=-lm -lgsl -lgslcblas -ldl

OBJS=$(OPTIMIZE) $(CONSTRAINTS) $(GRADIENT) $(TOOLS) $(EXTERNAL)

all: eDEa DE ratio

DE: DE.o main.o $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o DE DE.o main.o $(OBJS) $(LIBS) $(CLIBS)

eDEa: eDEa.o main.o $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o eDEa eDEa.o main.o $(OBJS) $(LIBS) $(CLIBS)

ratio: ratio.o $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o ratio ratio.o DE.o $(OBJS) $(LIBS) $(CLIBS)

clean:
	rm -f DE.o eDEa.o main.o ratio.o $(OBJS)
	rm -f *.bak */*.bak

main.o $(OPTIMIZE): eOPT.h epsilon/epsilon.h lib/rand.h
problem.o constraints.o: $(PROBLEMS)
nabla.o: $(NABLAS)
params.o: params.def
$(CONSTRAINTS): eOPT.h epsilon/epsilon.h epsilon/epsilon.def
$(GRADIENT): eOPT.h nabla.h gradient/gradient.h gradient/gradient.def lib/rand.h
DE.o: DE.h lib/rand.h DE.def
eDEa.o: eDEa.h lib/rand.h DE.def eDEa.def

# CEC2010 for WINDOWS
#CEC2010/fcnsuite.o: CEC2010/fcnsuite.c
#	$(CC) $(CFLAGS) -DWINDOWS -c CEC2010/fcnsuite.c -o CEC2010/fcnsuite.o
#CEC2010/fcnsuite.dll: CEC2010/fcnsuite.o
#	dllwrap -o CEC2010/fcnsuite.dll CEC2010/fcnsuite.o
# CEC2010 for LINUX
CEC2010/fcnsuite.o: CEC2010/fcnsuite.c
	$(CC) $(CFLAGS) -c CEC2010/fcnsuite.c -o CEC2010/fcnsuite.o
CEC2010/fcnsuite.dll: CEC2010/fcnsuite.o
	ld -o CEC2010/fcnsuite.so -shared CEC2010/fcnsuite.o
