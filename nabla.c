#include <math.h>
#include "eOPT.h"

#ifdef GRADIENT
#define DELTA	1.0e-12
#define DELTA_1	1.0e12

// input: x[] and c[], output: nabla
void EvaluateNabla(VARIABLE x[], int n, VALUE c[], VALUE nabla[][n],
	VALUE dg[][n], VALUE dh[][n]) // dummy variables
{
// Dummy variables are used, because the following declarations not work:
//	VALUE (*dg)[n]=&nabla[0];
//	VALUE (*dh)[n]=&nabla[Inequalities];

	dg=&nabla[0];
	dh=&nabla[Inequalities];

	NGradient++;
#ifndef NABLA_BY_CONSTRAINTS
	switch(Problem) {
//#include "problems/sample-nabla"
	default: // Calculate gradient matrix Numerically
#endif
	{
		VALUE c_dash[Constraints], ignore;
		VARIABLE xj;
		int i, j;

	// nabla constraints
		for(j=0; j<Length; j++) {
			xj=x[j];
			x[j]+=DELTA;
#ifdef EVALUATE_FC
			EvaluateFC(x, &ignore, c_dash);
#else
			EvaluateC(x, c_dash);
#endif
			NConst++;
			x[j]=xj;
			for(i=0; i<Constraints; i++)
				nabla[i][j]=(c_dash[i]-c[i])*DELTA_1;
		}
	}
#ifndef NABLA_BY_CONSTRAINTS
	}
#endif
}
#endif
