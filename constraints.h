/* number of constraints */
global int Constraints;		/* inequalities+equalities */
global int Inequalities;	/* number of inequalities */
global int Equalities;		/* number of equalities */
global int NConst;		/* number of constraints evaluations */
global Range *CRange;		/* ranges of constraints values */

global double Delta;		/* ignoring error smaller than Delta */
global int EvalFFlag;		/* don't skip function evaluation */

/* constraints function phi */
#define PHI(e)			(e)

/* constraints.c */
void EvaluateC(VARIABLE x[], VALUE c[]);
void ConstraintViolation(VARIABLE x[], VALUE c[],
	VALUE *error_p, VALUE *violation_p);
void ConstraintInit(Individual P, int n);
void ConstraintNormalize(Individual P, int n);
