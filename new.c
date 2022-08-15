#include <stdio.h>
#include <stdlib.h>
#include "eOPT.h"

void *NewCells(int size, int n, char *cell)
{
	void *new;

	new=(void *)malloc(size*n);
	if(new==NULL) {
		fprintf(stderr, "Cannot allocate memory for %d %s\n", n, cell);
		exit(1);
	}
	return new;
}

Individual NewIndividuals(int n)
{
	int i;
	Individual I;

	I=New(IndividualRec, n, "individuals");
	for(i=0; i<n; i++) {
		I[i].x=New(VARIABLE, Length, "genes");
#ifdef MULTIOBJECTIVE
		I[i].f=New(VALUE, Objective, "objectives");
#endif
#ifdef HOLD_CONSTRAINTS
		I[i].c=New(VALUE, Constraints, "constraints");
#endif
	}
	return(I);
}
