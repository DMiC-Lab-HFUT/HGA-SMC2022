#define IN	1
void RangeKeeperByReflection(VARIABLE x[])
{
	int j;
	int outside=0;
	double exceed;

	for(j=0; j<Length; j++) {
		if(x[j]<Ranges[j].min) {
			exceed=Ranges[j].min-x[j];
			if(exceed>Ranges[j].width)
				exceed-=((int)(exceed/Ranges[j].width))*Ranges[j].width;
			x[j]=Ranges[j].min+IN*exceed;
			outside=1;
		}
		else if(x[j]>Ranges[j].max) {
			exceed=x[j]-Ranges[j].max;
			if(exceed>Ranges[j].width)
				exceed-=((int)(exceed/Ranges[j].width))*Ranges[j].width;
			x[j]=Ranges[j].max-IN*exceed;
			outside=1;
		}
	}
	NOutside+=outside;
}
