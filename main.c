#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAINPROGRAM
#include "eOPT.h"
#include "lib/rand.h"

#ifdef SUCCESS_CHECK
static int isSuccess;
#endif

void Updated(Individual P)
{
	int updated=0, isfirst=0;

	if(Best->state<0) {
		isfirst=1;
#ifdef SUCCESS_CHECK
		isSuccess=0;
#endif
	}

/* update best individuals */
	if(Best->state<0 || (BetterNormal(P, Best)
#ifdef CONSTRAINTS
		&& (Evaluations==0 || NConst<=Evaluations)
#else
		&& (Evaluations==0 || NEval<=Evaluations)
#endif
	)) {
		Copy(Best, P);
		BestGen=Gen;
		BestEval=NEval;
#ifdef CONSTRAINTS
		BestConst=NConst;
#ifdef GRADIENT
		BestGradient=NGradient;
#endif
#endif
#ifdef SUCCESS_CHECK
		if(Success>=0 && !isSuccess && Best->error==0) {
#ifdef CONSTRAINTS
			UpdateIndividual(Best);
#endif
			if(MaximizationFlag && Optimal-Best->f<Success
				|| !MaximizationFlag && Best->f-Optimal<Success) {
				printf("Success %d %d %d %g %g %d %d %d %d\n",
					NEval, NConst, NGradient,
					Best->f, Optimal,
					SuccessEval, FailEval,
					SuccessSkip, FailSkip);
				isSuccess=1;
			}
		}
#endif
		updated=1;
	}
#ifdef CEC_CONTEST
	InformationCEC(Best, isfirst, updated);
#endif
}

int main(int argc, char *argv[])
{
int count;
count=0;
	FILE* fp;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
	//The output file.
	if ((fp = fopen("data/results.txt", "a"))
		== NULL) {
		printf("cannot open this file\n");
		exit(0);
	}
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
	int tmpargc = argc;
	int times = 1;
	int max_times = 1;
	unsigned long seed;
		seed=Random0();
//		printf("seed=%u\n", seed);
		SetRand(seed);
		for (; times <= max_times; times = times + 1) {
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			Individual Pop, New, Temp;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
//the key code to the segmentation fault
			Parameters(argc, argv);	/* InitOptimizer() is called */
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "\n\nProblem:C%d\n\n", Problem);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			printf("\n\nProblem:C%d\n\n", Problem);
			Pop = NewIndividuals(Popsize * 2);
			New = &Pop[Popsize];
			Best = NewIndividuals(1);
			for (Exp = 1; Exp <= Experiments; Exp++) {

				// Initialize parameters
				Gen = NEval = 0;
#ifdef CONSTRAINTS
				NConst = NOutside = 0;
#ifdef GRADIENT
				NGradient = 0;
#endif
#endif
#ifdef SUCCESS_CHECK
				SuccessEval = FailEval = SuccessSkip = FailSkip = 0;
#endif
				Best->state = -1;

#ifdef NORMALIZE_CONSTRAINTS
				ConstraintInit(Pop, Popsize);
#endif
				Initialize(Pop, Popsize);
#ifdef NORMALIZE_CONSTRAINTS
				ConstraintNormalize(Pop, Popsize);
#endif
#ifdef CONSTRAINTS
				if (EpsilonFlag)
					Epsilon = Epsilon0(Pop, Popsize);
#endif
				if (Report > 0)
					Statistics(Gen, Pop, Popsize);

				for (Gen = 1; ; Gen++) {
#ifdef DEBUG
					Print(Pop, Popsize);
#endif
					if (Generations > 0 && Gen >= Generations) break;
#ifdef CONSTRAINTS
					if (Evaluations > 0 && NConst >= Evaluations) break;
#endif
					if (Evaluations > 0 && NEval >= Evaluations) break;
#ifdef SUCCESS_CHECK
					if (StopFlag && isSuccess) break;
#endif

					Optimizer(Pop, Popsize, New);

					if (SwapPopulationFlag) {
						Temp = Pop; Pop = New; New = Temp;
					}
#ifdef CONSTRAINTS
#ifdef NORMALIZE_CONSTRAINTS
					ConstraintNormalize(Pop, Popsize);
#endif
					if (EpsilonFlag && Gen < EpsilonGenerations)
						Epsilon = EpsilonT(Gen, Pop, Popsize);
					else	Epsilon = 0;
#endif
					if (Report > 0 && Gen % Report == 0)
						Statistics(Gen, Pop, Popsize);
				}
				if (Report >= 0) {
					printf("Gen %d ", BestGen);
#ifdef CONSTRAINTS
#ifdef GRADIENT
					printf("Eval %d %d Const %d %d Grad %d %d Out %d ",
						BestEval, NEval, BestConst, NConst,
						BestGradient, NGradient, NOutside);
#else
					printf("Eval %d %d Const %d %d Out %d ",
						BestEval, NEval, BestConst, NConst, NOutside);
#endif
#else
					printf("Eval %d ", NEval);
#endif
#ifdef CONSTRAINTS
					UpdateIndividual(Best);
#endif
					Print(Best, 1);
				}
			}
			/***********Report***********/
			static int report10[] = { 20000, 100000, 200000, 0 };
			static int report30[] = { 60000, 300000, 600000, 0 };
			static int report_id;
			static int* report;
			report = (Length == 10 ? report10 : report30);
			report_id = 0;
			int i = 0, j = 0;
			//printf("FES1th\n");
			/***********FES1th***********/
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "\nFES%d\n", report[report_id++]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			/*sort the individuals according the principle that:
			 * compare A and B
			 * 	if the number of violated constraints of A is small than that of B ,
			 * 	or the number of violated constraints of A is equal to B,but the A's fitness is small
			 * 	A is better
			 * */
			for (i = 1; i <= Experiments; i++) {
				for (j = i + 1; j <= Experiments; j++) {
					bestRec tmp;
					if (c1[Bestf1[i].index][0] > c1[Bestf1[j].index][0]
						|| (c1[Bestf1[i].index][0]
							== c1[Bestf1[j].index][0]
							&& Bestf1[i].bestf > Bestf1[j].bestf)) {
						tmp = Bestf1[i];
						Bestf1[i] = Bestf1[j];
						Bestf1[j] = tmp;
					}
				}
			}
			int bestIndex = Bestf1[1].index;
			int median = Bestf1[Experiments / 2].index;
			int worstIndex = Bestf1[Experiments].index;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Best:%12.6e(%d)\n", Bestf1[1].bestf,
				c1[bestIndex][0]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Median:%12.6e(%d)\nWorst:%12.6e(%d)\n",
				Bestf1[Experiments / 2].bestf, c1[median][0],
				Bestf1[Experiments].bestf, c1[worstIndex][0]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "c:%d %d %d\n", c1[median][1], c1[median][2],
				c1[median][3]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			double meanv = 0.0;
			meanv = v1[median];
			fprintf(fp, "v:%12.6e\n", meanv);
			double meanf = 0.0;
			for (i = 1; i <= Experiments; i++)
				meanf += Bestf1[i].bestf;
			meanf /= Experiments;
			double s = 0.0;
			for (i = 1; i <= Experiments; i++)
				s += (Bestf1[i].bestf - meanf) * (Bestf1[i].bestf - meanf);
			s = sqrt(s / Experiments);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "mean:%12.6e\nstd:%12.6e\n", meanf, s);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			//printf("FES2th\n");
			/***********FES2th***********/
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "\nFES%d\n", report[report_id++]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			for (i = 1; i <= Experiments; i++) {
				for (j = i + 1; j <= Experiments; j++) {
					bestRec tmp;
					if (c2[Bestf2[i].index][0] > c2[Bestf2[j].index][0]
						|| (c2[Bestf2[i].index][0]
							== c2[Bestf2[j].index][0]
							&& Bestf2[i].bestf > Bestf2[j].bestf)) {
						tmp = Bestf2[i];
						Bestf2[i] = Bestf2[j];
						Bestf2[j] = tmp;
					}
				}
			}
			bestIndex = Bestf2[1].index;
			median = Bestf2[Experiments / 2].index;
			worstIndex = Bestf2[Experiments].index;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Best:%12.6e(%d)\n", Bestf2[1].bestf,
				c2[bestIndex][0]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Median:%12.6e(%d)\nWorst:%12.6e(%d)\n",

				Bestf2[Experiments / 2].bestf, c2[median][0],
				Bestf2[Experiments].bestf, c2[worstIndex][0]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "c:%d %d %d\n", c2[median][1], c2[median][2],
				c2[median][3]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			meanv = v2[median];

			fprintf(fp, "v:%12.6e\n", meanv);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			meanf = 0.0;
			for (i = 1; i <= Experiments; i++)
				meanf += Bestf2[i].bestf;
			meanf /= Experiments;
			s = 0.0;
			for (i = 1; i <= Experiments; i++)
				s += (Bestf2[i].bestf - meanf) * (Bestf2[i].bestf - meanf);
			s = sqrt(s / Experiments);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "mean:%12.6e\nstd:%12.6e\n", meanf, s);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			/***********FES3th***********/
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "\nFES%d\n", report[report_id++]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			for (i = 1; i <= Experiments; i++) {
				for (j = i + 1; j <= Experiments; j++) {
					bestRec tmp;
					if (c3[Bestf3[i].index][0] > c3[Bestf3[j].index][0]
						|| (c3[Bestf3[i].index][0]
							== c3[Bestf3[j].index][0]
							&& Bestf3[i].bestf > Bestf3[j].bestf)) {
						tmp = Bestf3[i];
						Bestf3[i] = Bestf3[j];
						Bestf3[j] = tmp;
					}
				}
			}
			bestIndex = Bestf3[1].index;
			median = Bestf3[Experiments / 2].index;
			worstIndex = Bestf3[Experiments].index;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Best:%12.6e(%d)\n", Bestf3[1].bestf,
				c3[bestIndex][0]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Median:%12.6e(%d)\nWorst:%12.6e(%d)\n",
				Bestf3[Experiments / 2].bestf, c3[median][0],
				Bestf3[Experiments].bestf, c3[worstIndex][0]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "c:%d %d %d\n", c3[median][1], c3[median][2],
				c3[median][3]);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			meanv = v3[median];
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "v:%12.6e\n", meanv);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			meanf = 0.0;
			for (i = 1; i <= Experiments; i++) {
				meanf += Bestf3[i].bestf;
			}
			meanf /= Experiments;
			s = 0.0;
			for (i = 1; i <= Experiments; i++)
				s += (Bestf3[i].bestf - meanf) * (Bestf3[i].bestf - meanf);
			s = sqrt(s / Experiments);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "mean:%12.6e\nstd:%12.6e\n", meanf, s);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			/***********Feasible Rate(%)***********/
			int numFea = 0, numInfea = 0;
			for (i = 1; i <= Experiments; i++) {
				if (v3[i] == 0.0)
					numFea++;
				else
					numInfea++;
			}
			FeasibleRate = (double)numFea / Experiments * 100;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Feasible Rate(%%):%.1f\n", FeasibleRate);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);

			/***********Eval/Grad Rate(%)***********/
			int numGrad = 0;
			for (i = 1; i <= Experiments; i++) {
				numGrad += NGradient3[i];
			}
			GradRate = (double)numGrad / (Experiments * Evaluations) * 100;

			int numEval = 0;
			for (i = 1; i <= Experiments; i++) {
				numEval += NEval3[i];
			}
			EvalRate = (double)numEval / (Experiments * Evaluations) * 100;
printf("niubi");
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "Eval/Grad Rate(%%):%.2f/%.4f\n", EvalRate, GradRate);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
			fprintf(fp, "numGrad:%d\n", numGrad);
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,%d\n",++count);
//fclose(fp);

for(i=0; i<Popsize*2;i++){
free((void*) Pop[i].c);
free((void*) Pop[i].x);
}
free((void*) Pop);

		}
	fclose(fp);
	ExitProblem();
	return 0;
}

static void Statistics(int t, Individual P, int n)
{
	int i;
	int best, worst;
	double sum=0.0;
#ifdef CONSTRAINTS
	double sum_error=0.0;
#endif

	best=worst=0;
	for(i=0; i<n; i++) {
		sum+=P[i].f;
#ifdef CONSTRAINTS
		sum_error+=P[i].error;
#endif
		if(Better(&P[i], &P[best])) best=i;
		else if(Better(&P[worst], &P[i])) worst=i;
	}
#ifdef CONSTRAINTS
	printf("+%d %d %d %d %g %.8g/%g %.8g/%g %.8g/%g\n",
			Exp, t, NEval, NConst, Epsilon,
			P[best].f, P[best].error,
			sum/n, sum_error/n,
			P[worst].f, P[worst].error);
#else
	printf("+%d %d %.8g %.8g %.8g\n",
		Exp, t, P[best].f, sum/n, P[worst].f);
#endif
}
