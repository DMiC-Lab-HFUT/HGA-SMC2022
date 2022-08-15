/* problem */
#define MAX_PROBLEM_NO			1000
#define DEFAULT_PROBLEM			1
typedef struct {
	char *name;
	int variables;
	int inequalities;
	int equalities;
	int maximization;
	VALUE optimal;
} Definition;
typedef struct {
	VARIABLE min, max, width;
} Range;

#ifdef MINIMIZATION
#define better(f1, f2)			(f1<f2)
#else
#define better(f1, f2)			(MaximizationFlag? f1>f2 : f1<f2)
#endif
//#define betterEQ(f1, f2)	(MaximizationFlag? f1>=f2 : f1<=f2)

/* problem: problems.c */
global int Problem init(DEFAULT_PROBLEM);	/* ID of problem */
global int Length init(0);		/* length of chromosome */
global VARIABLE Lower init(0.0);	/* lower bound */
global VARIABLE Upper init(0.0);	/* upper bound */
global Definition *Defs;		/* definition of problem */
global Range *Ranges;			/* range of each variable */
global int MaximizationFlag;		/* maximization */
global VALUE Optimal;			/* optimal value */
void InitProblem(void);
void ExitProblem(void);
void ShowProblems(void);
global void (*CallProblem)(double x[], double *y);
