#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eOPT.h"

#ifdef CONSTRAINTS
#ifndef EVALUATE_FC
void EvaluateC(VARIABLE x[], VALUE c[])
{
	VALUE *g=c, *h=c+Inequalities;

	switch(Problem) {
#include "problems/sample-gh"
	default:
		printf("Illegal problem number %d for constraint function\n", Problem);
		exit(1);
	}
}
#endif

void ConstraintViolation(VARIABLE x[], VALUE c[],
	VALUE *error_p, VALUE *violation_p)
{
	int i;
	double e, error, violation;

	error=violation=0.0;
	for(i=0; i<Length; i++) {
		e=Ranges[i].min-x[i];
		if(e>violation) violation=e;
		if(e>0) error+=PHI(e);
		e=x[i]-Ranges[i].max;
		if(e>violation) violation=e;
		if(e>0) error+=PHI(e);
	}
	for(i=0; i<Inequalities; i++) {
		if(c[i]>0) {
			if(c[i]>violation) violation=c[i];
			error+=PHI(c[i]);
		}
	}
	for(; i<Constraints; i++) {
		e=(c[i]>=0? c[i] : -c[i]);
		if(e>violation) violation=e;
		if(e>Delta) {
			error+=PHI(e-Delta);
		}
	}
	*error_p=error;
	*violation_p=violation;
}
#endif
