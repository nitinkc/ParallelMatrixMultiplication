#include <omp.h>
#include <stdio.h>
void main ()
{

	#pragma omp for
	for(int n=0; n<10; ++n)
	{
	  printf(" %d", n);
	}
	printf(".\n");

}
