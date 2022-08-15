global void (*CallRangeKeeper)(double x[]);
global char *RangeKeeperName init("reflection");
global int NOutside;			/* number of generated outside points */

int OutOfRange(VARIABLE x[]);
void InitRangeKeeper(void);
void ShowRangeKeepers(void);
void RangeKeeperByReflection(double x[]);
void RangeKeeperByBoundary(double x[]);
