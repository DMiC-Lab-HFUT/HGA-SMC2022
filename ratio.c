#include <stdio.h>
#include <stdlib.h>
#include "lib/rand.h"
#define MAINPROGRAM
#include "eOPT.h"

#define MAX	1000
#define UNIT	10000

void Updated(Individual P) {}

double ConstraintError(VARIABLE x[])
{
	VALUE error, violation;

	if(Constraints>0) {
		VALUE C[Constraints];

#ifdef EVALUATE_FC
		double ignore;
		EvaluateFC(x, &ignore, C);
#else
		EvaluateC(x, C);
#endif
		ConstraintViolation(x, C, &error, &violation);
	}
	else	error=0.0;
	return error;
}

int main(int argc, char **argv)
{
	int i, l, j;
	int satisfy=0;
	VARIABLE *x;

	Parameters(argc, argv);
	x=NewVariables();
	for(i=0; i<MAX; i++) {
	    printf("%04d/%04d\r", i, MAX); fflush(stdout);
	    for(j=0; j<UNIT; j++) {
		for(l=0; l<Length; l++) {
			x[l]=Ranges[l].min+Ranges[l].width*Rand();
		}
		if(ConstraintError(x)==0.0) satisfy++;
	    }
	}
	printf("\n%d/%.0f (%f%%)\n", satisfy, MAX*(double)UNIT, 100.0/UNIT*satisfy/MAX);
	return 0;
}
