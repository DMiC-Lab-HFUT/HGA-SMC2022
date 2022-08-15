/* special rules */
#ifdef CEC2010
#define MINIMIZATION
#define CONSTRAINTS
#define GRADIENT
#define FCNSUITE_DLL
#define EVALUATE_FC
#define NABLA_BY_CONSTRAINTS
#define CEC_CONTEST
#define HOLD_CONSTRAINTS
#endif

#ifdef SAMPLE
#define GRADIENT
#endif

/* common rules */
#ifdef GRADIENT
#ifndef CONSTRAINTS
#define CONSTRAINTS
#endif
#endif
