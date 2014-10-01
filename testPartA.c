#include <omp.h>
#include <stdio.h>
#define MAX 50
void main ()
{
	int n =0;
	for(n=0; n<MAX; ++n)
	{
	  printf(" %d", n);
	}
	printf(".\n");

	 n =0;
	 	int this_thread = omp_get_thread_num(), num_threads = omp_get_num_threads();
          int my_start = (this_thread  ) * MAX / num_threads;
          int my_end   = (this_thread+1) * MAX / num_threads;

         

#pragma omp parallel shared (this_thread,my_start,my_end,num_threads,n)
{

	 	 this_thread = omp_get_thread_num();
	 	 num_threads = omp_get_num_threads();
         my_start = (this_thread  ) * MAX / num_threads;
         my_end   = (this_thread+1) * MAX / num_threads;

		 printf("This_threads =  %d Num_threads = %d my_start = %d my_end = %d\n", this_thread, num_threads, my_start, my_end);
          for(n=my_start; n<my_end; ++n)
	   	  printf(" %d by Thread:%d\n", n, this_thread);
}//end parallel
}
