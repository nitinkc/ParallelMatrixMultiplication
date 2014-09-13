
	#include <time.h>
	#include <stdio.h>
#include<stdlib.h>
#include<unistd.h> 


	int main(void)
	{
		time_t mytime;
		mytime = time(NULL);
		printf(ctime(&mytime));
		printf("\n %s \n",getlogin());
	return 0;
	}
