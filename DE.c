#include <stdlib.h>
#include "lib/rand.h"
#include "eOPT.h"
#define DE_MAIN
#include "DE.h"

#define DE	Optimizer
#define DEsetup	InitOptimizer

void DE(Individual Pop, int n, Individual New)
{
	int i, best;
	int p1, p2, p3;

	if(Gen==1) {
		DEBest=0;
		for(i=1; i<n; i++)
			if(Better(&Pop[i], &Pop[DEBest])) DEBest=i;
	}

	best=DEBest;
	for(i=0; i<n; i++) {
		DEselect(p1, p2, p3);
		if(DEBinomialCrossoverFlag)
			DE1bin(New[i].x, Pop[i].x,
				Pop[p1].x, Pop[p2].x, Pop[p3].x)
		else
			DE1exp(New[i].x, Pop[i].x,
				Pop[p1].x, Pop[p2].x, Pop[p3].x)
		EvaluateIndividual(&New[i]);
		if(Better(&New[i], &Pop[i])) { /* new is better than old */
			if(Better(&New[i], &Pop[DEBest])) {
				if(!SwapPopulationFlag) DEBest=i;
				else best=i;
			}
			if(!SwapPopulationFlag)
				Copy(&Pop[i], &New[i]);
			SuccessEval++;
		}
		else {
			if(SwapPopulationFlag)
				Copy(&New[i], &Pop[i]);
			FailEval++;
		}
	}
	if(SwapPopulationFlag) DEBest=best;
}

static OptionRec Options[]={
{ "-o DE", "options for Differential Evolution" },
#include "DE.def"
};
#define OPTIONS	Numberof(Options)

void DEsetup(int *argc, char *argv[], Option *O, int *n)
{
	*O=Options; *n=OPTIONS;
	AnalyzeParameters(*O, *n, argc, argv);
}
