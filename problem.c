#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/rand.h"
#include "eOPT.h"

void SetProblem(int problem)
{
	switch(problem) {
#ifdef SAMPLE
#include "problems/sample-def"
#else
#include "problems/cec2010-def"
#endif
	default:
		Defs=NULL;
	}
}

void InitProblem(void)
{
	int i;

	SetProblem(Problem);
	if(Defs==NULL) {
		printf("Illegal problem number %d\n", Problem);
		exit(1);
	}
	Optimal=Defs->optimal;
	if(Length==0)
		Length=Defs->variables;
#ifdef CONSTRAINTS
	Inequalities=Defs->inequalities;
	Equalities=Defs->equalities;
	Constraints=Inequalities+Equalities;
#endif
	MaximizationFlag=Defs->maximization;
	if(Ranges==NULL) {
		Ranges=NewRanges();
		for(i=0; i<Length; i++) {
			Ranges[i].min=Lower;
			Ranges[i].max=Upper;
		}
	}
	for(i=0; i<Length; i++) {
		Ranges[i].width=Ranges[i].max-Ranges[i].min;
#ifdef DEBUG
		printf("%2d: %g - %g = %g\n",
			i, Ranges[i].min, Ranges[i].max, Ranges[i].width);
#endif
	}
#ifdef FCNSUITE_DLL
	InitDLL();
#endif
}

void ExitProblem(void)
{
#ifdef FCNSUITE_DLL
	ExitDLL();
#endif
}

void ShowProblems(void)
{
	int problem;

	for(problem=0; problem<MAX_PROBLEM_NO; problem++) {
		SetProblem(problem);
		if(Defs!=NULL) {
			char *opt;
			opt=MaximizationFlag? "maximize" : "minimize";
#ifdef CONSTRAINTS
			printf("%3d: %s %-3s of %2d vars %d ineqs %d eqs, optimal is %g\n",
#else
			printf("%3d: %s %-3s of %2d vars, optimal is %g\n",
#endif
				problem, opt, Defs->name, Defs->variables,
#ifdef CONSTRAINTS
				Defs->inequalities, Defs->equalities,
#endif
				Defs->optimal);
		}
	}
}

#ifndef EVALUATE_FC
void EvaluateF(VARIABLE x[], VALUE *f)
{
	int i;

	switch(Problem) {
#include "problems/sample-f"
	default:
		printf("Illegal problem number %d for objective function\n", Problem);
		exit(1);
	}
}
#endif
