#include <omp.h>
#include <stdio.h>
#define MAX 100
void main ()
{
	int n =0;
//#pragma omp parallel
//	{
//	#pragma omp for
//	for(n=0; n<MAX; ++n)
//	{
//	  printf(" %d", n);
//	}
//	printf(".\n");
//	}//End Parallel

	  int this_thread = omp_get_thread_num(), num_threads = omp_get_num_threads();
	  int my_start = (this_thread  ) * MAX / num_threads;
	  int my_end   = (this_thread+1) * MAX / num_threads;
	  printf("This_threads =  %d\n",this_thread);
	  printf("Num_threads =  %d\n",num_threads);
	  printf("my_start =  %d\n",my_start);
	  printf("my_end =  %d\n",my_end);
	  for(n=my_start; n<my_end; ++n)
	   printf(" %d", n);
	printf(".\n");

}
