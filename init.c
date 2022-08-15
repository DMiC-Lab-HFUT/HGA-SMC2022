#include <stdlib.h>
#include "lib/rand.h"
#include "eOPT.h"

void Initialize(Individual P, int n)
{
	int i, j;

	for(i=0; i<n; i++) {
		for(j=0; j<Length; j++) {
			P[i].x[j]=Ranges[j].min+Ranges[j].width*Rand();
		}
		EvaluateIndividual(&P[i]);
	}
}
