#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lib/rand.h"
#include "eOPT.h"
#define DE_MAIN
#define eDEa_MAIN
#include "DE.h"
#include "eDEa.h"

#define eDEa		Optimizer
#define eDEaSetup	InitOptimizer

void eDEaGeneration(Individual Pop, int n, int M, Individual New)
{
	int i, k;
	int j, l, p1, p2, p3;

	for(i=0; i<n; i++) {
	    for(k=0; k<2; k++) {
		do {
			p1=RandInt(n);
		} while(p1==i);
		do {
			p2=RandInt(n);
		} while(p2==i || p2==p1);
		if(Rand()<0.05)
			do {
				p3=RandInt(n);
			} while(p3==i || p3==p1 || p3==p2);
		else
			do {
				p3=RandInt(M);
			} while(p3==i || p3==p1 || p3==p2);

		j=RandInt(Length);
		l=0;
		do {
			New->x[j]=Pop[p1].x[j]
				+DE_F*(Pop[p2].x[j]-Pop[p3].x[j]);
			j=(j+1)%Length;
			l++;
		}
		while(l<Length && Rand()<DE_CR);
		for(; l<Length; l++) {
			New->x[j]=Pop[i].x[j];
			j=(j+1)%Length;
		}
		EvaluateIndividual(New);
		if(Better(New, &Pop[i])) { /* new is better than old */
			Copy(&Pop[i], New);
			SuccessEval++;
			break;
		}
		else {
			l=n+RandInt(M-n);
			Copy(&Pop[l], New);
			FailEval++;
		}
	    }
	}
}

void eDEa(Individual Pop, int M, Individual New)
{
	int i, k, best;
	static int n;
	static double F0=0, EpsilonCP0, Gradient0;
	static Individual Work;

	if(F0==0) {
		F0=DE_F;
		Gradient0=GradientMutationRate;
		n=EffectivePopsize;
		Work=NewIndividuals(1);
	}
	DE_F=F0;
	GradientMutationRate=Gradient0;
	if(Gen==1) {
		/* elites according to <_epsilon */
		for(i=0; i<n; i++) {
			best=i;
			for(k=i+1; k<M; k++)
				if(Better(&Pop[k], &Pop[best])) best=k;
			if(best!=i) {
				Copy(&New[i], &Pop[best]);
				Copy(&Pop[best], &Pop[i]);
				Copy(&Pop[i], &New[i]);
			}
		}
		double order;
#define ECP_MIN		3
#define ECP_RATIO	0.95
#define ECP_VALUE	1e-5
// 10^{-5} at 0.95Tc
		order=(Epsilon>0? log10(Epsilon) : -100);
		EpsilonCP=(log10(ECP_VALUE)-order)/log10(1-ECP_RATIO);
		if(EpsilonCP<ECP_MIN) EpsilonCP=ECP_MIN;
		printf("e(0)=%g, cp=%g\n", Epsilon, EpsilonCP);
		EpsilonCP0=EpsilonCP;
	}
	else {
		double t=(double)Gen/EpsilonGenerations;

		if(t>=1) {
			Epsilon=0;
		}
		else {
			// seeking good objective values
			if(t>ECP_RATIO) {
#define WEIGHT 0.3
				EpsilonCP=WEIGHT*EpsilonCP+(1-WEIGHT)*ECP_MIN;
				DE_F=WEIGHT*F0+(1-WEIGHT)*1;
			}
			Epsilon=Epsilon_0*pow(1-t, EpsilonCP);
		}
	}
	if(Rand()<0.05) {
#define F_MAX	1.1
		DE_F=1+0.05*fabs(gaussrand());
		if(DE_F>F_MAX) DE_F=F_MAX;
	}
	eDEaGeneration(Pop, n, M, Work);
}

static OptionRec Options[]={
{ "-o eDEa", "options for epsilon Constrained Differential Evolution with an Archive" },
#include "eDEa.def"
};
#define OPTIONS	Numberof(Options)

void eDEaSetup(int *argc, char *argv[], Option *O, int *n)
{
	*O=Options; *n=OPTIONS;
	AnalyzeParameters(*O, *n, argc, argv);
	if(EffectivePopsize==0) EffectivePopsize=4*Length;
	if(Popsize<=EffectivePopsize) {
		printf("ERROR: Popsize must be greater than %d\n",
			EffectivePopsize);
		exit(1);
	}
}
