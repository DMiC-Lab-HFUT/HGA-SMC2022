#undef global
#undef init
#ifdef DE_MAIN
#define global
#define init(x)	= x
#else
#define global	extern
#define init(x)
#endif

global double DE_F init(0.5);
global double DE_CR init(0.9);
// base vector
global int DEBestFlag init(0);
// crossover
global int DEBinomialCrossoverFlag init(0);

global int DEBest;
void DE(Individual Old, int n, Individual New);

#define DEselect(p1, p2, p3) \
	if(DEBestFlag) \
		p1=DEBest; \
	else \
		do { \
			p1=RandInt(n); \
		} while(p1==i); \
	do { \
		p2=RandInt(n); \
	} while(p2==i || p2==p1); \
	do { \
		p3=RandInt(n); \
	} while(p3==i || p3==p1 || p3==p2)
#define DE1exp(trial, xi, x0, x1, x2) \
	{ \
		int j=RandInt(Length); \
		int l=0; \
		do { \
			trial[j]=x0[j]+DE_F*(x1[j]-x2[j]); \
			j=(j+1)%Length; \
			l++; \
		} \
		while(l<Length && Rand()<DE_CR); \
		for(; l<Length; l++) { \
			trial[j]=xi[j]; \
			j=(j+1)%Length; \
		} \
	}
#define DE1bin(trial, xi, x0, x1, x2) \
	{ \
		int j=RandInt(Length); \
		int l; \
		for(l=0; l<Length; l++) { \
			if(l==j || Rand()<DE_CR) \
				trial[l]=x0[l]+DE_F*(x1[l]-x2[l]); \
			else	trial[l]=xi[l]; \
		} \
	}
