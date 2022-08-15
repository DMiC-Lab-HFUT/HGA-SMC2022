#ifdef MAINPROGRAM
#define global
#define init(x)	= x
#else
#define global	extern
#define init(x)
#endif

#include "config.h"
#define VERSION				"2010/04/30"
#define LAST_UPDATE			"2010/02/22"

typedef double VALUE;
typedef double VARIABLE;

#include "problem.h"

/* individual */
typedef struct {
	VARIABLE *x;
#ifdef MULTIOBJECTIVE
	VALUE *f;
#else
	VALUE f;
#endif
	VALUE error;
	VALUE violation;
#ifdef HOLD_CONSTRAINTS
	VALUE *c;	// values of constraints
#endif
	int state;
	VALUE value;	// reserved for approximation
} IndividualRec, *Individual;

global Individual Best;	// Best individual

#define Random0()	xorshift32_rand()
#define SetRand(seed)	xorshift128_init(seed)
#define Rand()		xorshift128_rand1()
#define RandInt(n)	((int)(xorshift128_rand2()*(n)))

#define square(x)	((x)*(x))
#define cubic(x)	((x)*(x)*(x))
#define Numberof(x)	(sizeof(x)/sizeof(x[0]))

/* experiment */
global int Experiments init(25);		/* maximum experiments */
global int Exp;				/* current experiment */
global int Generations init(5000);	/* maximum generations */
global int Gen;				/* current generation */
global int Evaluations init(0);		/* maximum evaluations */
global int Popsize init(1000);		/* population size */
global VALUE Success init(-1);		/* precision of successful solution */
global int StopFlag init(0);		/* stop at successful solution */

/* flags */
global int SwapPopulationFlag;		/* generational model */

/* others */
global int NEval;
global int Report init(0);		/* show report each Report gen. */
global int LongReport init(0);		/* long format report each Long eval. */
global double Real init(0.0);		/* extra real parameter */
global int Integer init(0);		/* extra integer parameter */
global char *String init(NULL);		/* extra string parameter */

global int SuccessEval, FailEval;	/* number of success/fail eval */
global int SuccessSkip, FailSkip;	/* number of success/fail skip */
global double ApproxModelError;		/* MSE of approximation error */

/* options */
typedef struct {
	char *name, *message;
	int type;
	void *variable;
} OptionRec, *Option;
enum { OPTION_FLAG, OPTION_INTEGER, OPTION_REAL, OPTION_STRING };

/* sub-systems */
#include "rangekeepers.h"
#ifdef CEC2010
#include "CEC2010/cec2010.h"
#endif

/* optimizer */
void Optimizer(Individual Pop, int M, Individual New);
void InitOptimizer(int *argc, char *argv[], Option *O, int *n);

/* main.c */
void Updated(Individual P);

/* init.c */
void Initialize(Individual P, int n);

/* eval.c */
void EvaluateIndividual(Individual P);
void UpdateIndividual(Individual P);
global void EvaluateF(VARIABLE x[], VALUE *y);

typedef struct {
	double bestf;
	int index;
}bestRec, * best;
#define MaxExp 101 //Maxmum Run Times=MaxExp-1;
global  int BestGen, BestEval, BestConst, BestGradient;
global  int BestGen3[MaxExp], BestEval3[MaxExp], BestConst3[MaxExp], BestGradient3[MaxExp], NEval3[MaxExp], NGradient3[MaxExp];
global  int BestGen1[MaxExp], BestEval1[MaxExp], BestConst1[MaxExp], BestGradient1[MaxExp], NEval1[MaxExp], NGradient1[MaxExp];
global  int BestGen2[MaxExp], BestEval2[MaxExp], BestConst2[MaxExp], BestGradient2[MaxExp], NEval2[MaxExp], NGradient2[MaxExp];
global  bestRec Bestf1[MaxExp], Bestf2[MaxExp], Bestf3[MaxExp];
global  double v1[MaxExp], v2[MaxExp], v3[MaxExp];
global  int c1[MaxExp][4], c2[MaxExp][4], c3[MaxExp][4];
global double FeasibleRate init(0.0);
global double GradRate init(0.0);
global double EvalRate init(0.0);
static void Statistics(int t, Individual P, int n);

/* constraints.c */
#ifdef CONSTRAINTS
#include "constraints.h"
enum { STATE_C, STATE_F, STATE_NONE };
#ifdef NORMALIZE_CONSTRAINTS
global VALUE *CMax;
#endif
#else
global int NConst init(0);
#endif

/* epsilon: compare.c epsilon.c epsilon0.c epsilon_t.c */
#include "epsilon/epsilon.h"
#ifdef CONSTRAINTS
#define Better(p1, p2)		BetterEpsilon(p1, p2, Epsilon)
#define BetterLambda(p1, p2)	BetterEpsilonLambda(p1, p2, Epsilon, Lambda)
#else
#define Better(p1, p2)		better((p1)->f, (p2)->f)
#define BetterNormal(p1, p2)	better((p1)->f, (p2)->f)
#define BetterLambda(p1, p2)	better_lambda((p1)->f, (p2)->f, Lambda)
#endif

/* gradient.c */
#ifdef GRADIENT
#include "gradient/gradient.h"
#else
global int NGradient init(0);
#endif

/* lambda.c */
#ifdef LAMBDA_COMPARISON
#include "lambda/lambda.h"
#endif

/* new.c */
void *NewCells(int size, int n, char *cell);
#define New(type, n, cell)	(type *)NewCells(sizeof(type), n, cell)
#define NewVariables()	New(VARIABLE, Length, "variables")
#define NewRanges()	New(Range, Length, "ranges")
Individual NewIndividuals(int n);

/* menu.c */
typedef struct {
	char *name, *message;
	int id;
	void (*call)();
} MenuRec, *Menu;

void ShowMenu(MenuRec menu[], int n);
int SelectMenu(char *name, MenuRec menu[], int n);

/* params.c */
void Parameters(int argc, char *argv[]);
void AnalyzeParameters(Option opt, int n, int *argc, char *argv[]);
void ShowParameters(Option opt, int n);
void HelpParameters(Option opt, int n);

/* utils.c */
void Copy(Individual dest, Individual src);
void Print(Individual Pop, int n);
void SortPopF(int order[], Individual Pop, int n);
void RankPopF(int rank[], Individual Pop, int n);

/* version.c */
void version(void);
