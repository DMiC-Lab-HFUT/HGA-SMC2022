#undef global
#undef init
#ifdef eDEa_MAIN
#define global
#define init(x)	= x
#else
#define global	extern
#define init(x)
#endif

void eDEaSetup(int *argc, char *argv[], Option *O, int *n);
void eDEa(Individual Old, int n, Individual New);
global int EffectivePopsize;
