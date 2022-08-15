#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "eOPT.h"

#define string(s)		(s==NULL? "NULL" : s)
#define OPTIONS_PER_LINE	7

void HelpParameters(Option option, int n)
{
	int k;

	printf("%s: %s\n", option[0].name, option[0].message);
	for(k=1; k<n; k++) {
		switch(option[k].type) {
		case OPTION_FLAG:
			printf("%s: %s\n", option[k].name, option[k].message);
			break;
		case OPTION_INTEGER:
			printf("%s <int>: %s (current %d)\n",
					option[k].name, option[k].message,
					*(int *)option[k].variable);
			break;
		case OPTION_REAL:
			printf("%s <real>: %s (current %g)\n", 
					option[k].name, option[k].message,
					*(double *)option[k].variable);
			break;
		case OPTION_STRING:
			printf("%s <string>: %s (current %s)\n",
					option[k].name, option[k].message,
					string(*(char **)option[k].variable));
			break;
		}
	}
}

void ShowParameters(Option option, int n)
{
	int k;

	printf("%s: %s\n", option[0].name, option[0].message);
	for(k=1; k<n; k++) {
		switch(option[k].type) {
		case OPTION_FLAG:
			if(*(int *)option[k].variable)
				printf("%s ", option[k].name);
			break;
		case OPTION_INTEGER:
			printf("%s %d ", option[k].name,
					*(int *)option[k].variable);
			break;
		case OPTION_REAL:
			printf("%s %g ", option[k].name,
					*(double *)option[k].variable);
			break;
		case OPTION_STRING:
			printf("%s %s ", option[k].name,
					string(*(char **)option[k].variable));
			break;
		}
		if(k%OPTIONS_PER_LINE==0) printf("\n");
	}
	printf("\n\n");
}

void AnalyzeParameters(Option option, int n, int *argc, char *argv[])
{
	int i, k;
	int args=1;

	for(i=1; i<*argc; i++) {
	    for(k=1; k<n; k++)
		if(strcmp(argv[i], option[k].name)==0) {
			if(option[k].type!=OPTION_FLAG && i+1==(*argc)) {
				k=n; break;
			}
			switch(option[k].type) {
			case OPTION_FLAG:
				*(int *)option[k].variable=1;
				break;
			case OPTION_INTEGER:
				*(int *)option[k].variable=atoi(argv[++i]);
				break;
			case OPTION_REAL:
				*(double *)option[k].variable=atof(argv[++i]);
				break;
			case OPTION_STRING:
				*(char **)option[k].variable=argv[++i];
				break;
			}
			break;
		}
	    if(k==n) argv[args++]=argv[i];
	}
	argv[args]=NULL;
	*argc=args;
}

static int show_problems=0;
static int show_optimizers=0;
static int show_range_keepers=0;
#ifdef CONSTRAINTS
static int show_epsilon=0;
#endif
static int show_help=0;
static int show_version=0;

OptionRec CommonOptions[]={
#include "params.def"
};
#define OPTIONS	Numberof(CommonOptions)

void Parameters(int argc, char *argv[])
{
	Option opt_options; int opt_n;

	AnalyzeParameters(CommonOptions, OPTIONS, &argc, argv);
	if(show_problems) ShowProblems();
	if(show_help) HelpParameters(CommonOptions, OPTIONS);
	if(show_version) version();

	InitProblem();
	InitOptimizer(&argc, argv, &opt_options, &opt_n);
#ifdef CONSTRAINTS
	Option eps_options; int eps_n=0;
	InitEpsilon(&argc, argv, &eps_options, &eps_n);
#ifdef GRADIENT
	Option grad_options; int grad_n=0;
	InitGradientMutation(&argc, argv, &grad_options, &grad_n);
#endif
#endif
	if(show_help) {
		printf("----- parameters for optimizer -----\n");
		HelpParameters(opt_options, opt_n);
#ifdef CONSTRAINTS
		printf("----- parameters for epsilon constrained method -----\n");
		HelpParameters(eps_options, eps_n);
#ifdef GRADIENT
		printf("----- parameters for gradient mutation -----\n");
		HelpParameters(grad_options, grad_n);
#endif
#endif
	}
	if(show_problems || show_optimizers || show_range_keepers
		|| show_help || show_version
#ifdef CONSTRAINTS
		|| show_epsilon
#endif
		)
		exit(0);

	if(argc>1) {
		int i;
		printf("ERROR: followings are unfedined options:\n");
		for(i=1; i<argc; i++)
			printf("%s ", argv[i]);
		printf("\n");
		printf("Please type '%s -h' for more information.\n", argv[0]);
		exit(1);
	}

	Popsize-=Popsize%2;
	if(Report>=0) {	// if Report is negative, no message
		ShowParameters(CommonOptions, OPTIONS);
		ShowParameters(opt_options, opt_n);
#ifdef CONSTRAINTS
		ShowParameters(eps_options, eps_n);
#ifdef GRADIENT
		ShowParameters(grad_options, grad_n);
#endif
#endif
	}
}

