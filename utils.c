#include <stdio.h>
#include <math.h>
#include "eOPT.h"

void Copy(Individual dest, Individual src)
{
	int j;

	for(j=0; j<Length; j++) {
		dest->x[j]=src->x[j];
	}
#ifdef MULTIOBJECTIVE
	for(j=0; j<Objective; j++)
		dest->f[j]=src->f[j];
#else
	dest->f=src->f;
#endif
#ifdef CONSTRAINTS
	dest->error=src->error;
	dest->violation=src->violation;
#ifdef HOLD_CONSTRAINTS
	for(j=0; j<Constraints; j++) {
		dest->c[j]=src->c[j];
	}
#endif
#endif
	dest->state=src->state;
}

void Print(Individual P, int n)
{
	int i, j;

	for(i=0; i<n; i++) {
		printf("%2d ", i+1);
		for(j=0; j<Length; j++)
			printf("%.8g ", P[i].x[j]);
#ifdef CONSTRAINTS
#ifdef HOLD_CONSTRAINTS
		for(j=0; j<Constraints; j++)
			printf("%g ", P[i].c[j]);
#endif
		printf("=> %.15g %.8g %.8g\n", P[i].f, P[i].error, P[i].violation);
#else
		printf("=> %.15g\n", P[i].f);
#endif
	}
}
