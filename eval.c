#include <stdio.h>
#include "lib/rand.h"
#include "eOPT.h"
#include "reflection.c"

void EvaluateIndividual(Individual ind)
{
	RangeKeeperByReflection(ind->x);

#ifdef CONSTRAINTS
	if(Constraints>0) {
#ifdef HOLD_CONSTRAINTS
		VALUE *C=ind->c;
#else
		VALUE C[Constraints];
#endif
		ind->state=STATE_NONE;
#ifdef GRADIENT
		if(GradientMutationFlag
#ifdef NABLA_BY_CONSTRAINTS
			&& (Gen>0 && Gen%Length==0)
#endif
			&& Rand()<GradientMutationRate)
				GradientMutation(ind);
#endif
		if(ind->state==STATE_NONE) {
#ifdef EVALUATE_FC
			EvaluateFC(ind->x, &ind->f, C);
#else
			EvaluateC(ind->x, C);
#endif
			NConst++;
			ConstraintViolation(ind->x, C,
				&ind->error, &ind->violation);
		}
	}
	else
		ind->error=ind->violation=0;
#ifndef EVALUATE_FC
	if(EvalFFlag) {
		EvaluateF(ind->x, &ind->f);
		NEval++;
		ind->state=STATE_F;
	}
	else {
#endif
		ind->state=STATE_C;
#ifndef EVALUATE_FC
	}
#endif
#else
	EvaluateF(ind->x, &ind->f);
	NEval++;
#endif
	Updated(ind);
}

#ifdef CONSTRAINTS
void UpdateIndividual(Individual ind)
{
	if(ind->state==STATE_C) {
#ifndef EVALUATE_FC
		EvaluateF(ind->x, &ind->f);
#endif
		NEval++;
		ind->state=STATE_F;
	}
}
#endif
