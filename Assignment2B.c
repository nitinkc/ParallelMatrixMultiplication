/*
 * Assignment2.c
 *
 *  Created on: Sep 30, 2014
 *      Author: nitin
 */


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

//Random Number Range
#define MAXRAND 9999
#define TRUE 1
#define FALSE 0

//Dimensions of the Matrices to be multiplied
#define ROW_A 50
#define COL_A 20
#define ROW_B 20
#define COL_B 25

/* GLOBAL VARIABLE DECLAARTION */

	/* Pointer for the three Matrix*/
		double *matA=NULL;
		double *matB=NULL;
		double *matC=NULL;

	/* Variables for sequential execution of time*/
		clock_t begin_seq=0, end_seq=0;
		double time_spent_seq=0;

	/* Variables for Parallel execution time*/
		double begin=0, end=0;
		double time_spent=0;

	/* Declare number of Threads for Parallel Execution*/
		int numThreads = 1;
		int chunk, threadId;

	/* Function Prototypes */
		void allocateMemory();
		void fillMatrix();
		void parallelMultiplication();
		void collectResults();

/* MAIN BEGINS */
int main(){

	/* TEMP : WELCOME MESSAGE AND ASKING FOR USER DEFINED NUMDER OF THREADS*/
	printf("=============================WELCOME============================\n");
	printf("            LAB ASSIGNMENT 2B (Row-wise Dynamic Scheduling      \n");
	printf("================================================================\n\n");
	printf("Enter Number of Threads: ");
	scanf("%d", &numThreads);
	printf("The number of threads are %d\n", numThreads);

	/* Check for multiplication compatibility */
		if (COL_A != ROW_B){
			printf("Column for Matrix A should be same as that of Row for Matrix B\n");
			printf("Multiplication is not possible\n");
			return(0); //int main is used to control the untimely exit
		}

		/* Allocate Memory for the Matrix and add Random Double values into it */
		allocateMemory();

		/* Generate Random numbers and fill them in the Matrix*/
		fillMatrix();

		/* Parallel Multiplication*/
		parallelMultiplication();

		//free all of the memory
	  printf("Freeing memory.....\n");
	  free(matA);
	  free(matB);
	  free(matC);

	/* Output FILE HANDLING */
	  collectResults();

  return 0;
} //end main()

/* This subroutine allocates the memory to the Matrices */
void allocateMemory(){
	int i,j,k;
	/* If Multiplication is Possible, allocate memory for the three matrices and check if its failed */
		 matA = malloc(ROW_A*COL_A*sizeof(double));
		  if(matA==NULL)
			  printf("\nMemory allocation failed for matrix A.\n");
		  matB = malloc(ROW_B*COL_B*sizeof(double));
		  if(matB==NULL)
			  printf("\nMemory allocation failed for matrix B.\n");
		  matC = malloc(ROW_A*COL_B*sizeof(double));
		  if(matC==NULL)
			  printf("\nMemory allocation failed for matrix C.\n");

		  printf("Memory Allocation Successfully Done!!\n");
}

void fillMatrix(){
	int i,j,k;
	/* Initialize the seed to generate Random Values */
	srand((unsigned)time(NULL));

	/* Generate Matrices in parallel*/
	//Matrix A
	for(i=0;i<ROW_A;i++){
		for(j=0;j<COL_A;j++){
			*(matA+(i*COL_A+j))=((double)rand()/(double)MAXRAND);
		}//End Loop for Column
	}//End Loop for Row

	//Matrix B
	for(i=0;i<ROW_B;i++){
		for(j=0;j<COL_B;j++){
			*(matB+(i*COL_A+j))=((double)rand()/(double)MAXRAND);
		}//End Loop for Column
	}//End Loop for Row

	//Matrix C (initialized to Zero)
	for(i=0;i<ROW_A;i++){
		for(j=0;j<COL_B;j++){
			*(matC+(i*COL_B+j)) = 0.00;
		}//End Loop for Column
	}//End Loop for Row
}

		/* For parallel threaded multiplication, Rows from matrix A is to be multiplied with Columns
		 * of matrix B resulting in the Rows of Matrix C. Thus each thread can work on some Rows,
		 * depending upon the division of jobs into threads  */

void parallelMultiplication(){
		int i,j,k;
		//Variables for
		int poolCounter = 0;
		int	assignIndex=TRUE;//variable to break the while loop after allocation of task is done
		omp_lock_t lock;
		begin = omp_get_wtime();
		

		omp_init_lock(&lock);//initialize lock on poolCounter
#pragma omp parallel shared(matA,matB,matC,poolCounter,assignIndex) private(i,j,k) num_threads(numThreads)
		{
			while(assignIndex){//Removal of First for loop and picking up individual row
				omp_set_lock(&lock);
				if(poolCounter < ROW_A){
					//printf("Thread =  %d takes Row = %d\n", omp_get_thread_num(),i);
					poolCounter++;//increment the pool counter until ROW_A
				}

				else{
					assignIndex = FALSE;
					omp_unset_lock(&lock);
					continue;
				}
				i = poolCounter;
				omp_unset_lock(&lock);
			 //Each thread gets one Row
				  for(j=0;j<COL_B;j++){  //column of second matrix
					  for(k=0;k<COL_A;k++){
						  *( matC+(i*COL_A+j) ) += *( matA+(i*ROW_A+k) )*( *( matB+(k*COL_B+j) ));
					}//end k
				  }//end j
			  }//end WHILE
		  }//Parallel block ends
		end = omp_get_wtime();
		time_spent = (double)(end - begin) ;
		printf("The time spent is : %1.5f\n", time_spent);
}

void collectResults(){

	/* Variable Declaration*/
	FILE *resultFilePointer;
	resultFilePointer = fopen ("Results2B.csv","a+"); //Append mode, Returns the File descriptor (Null pointer otherwise)
	if (resultFilePointer == NULL) {
		printf ("Cannot open file to write!\n");
		 exit(-1);
	}

	//For Current System time
	time_t mytime;
	fprintf(resultFilePointer,"Testing done on : %s", ctime(&mytime));
	printf("Parallel execution time of Matrices of dim %dX%d & %dX%d with %d no. of threads is %f\n", ROW_A, COL_A, ROW_B, COL_B, numThreads, time_spent);
	fprintf (resultFilePointer, "Parallel execution time of Matrices of dim %dX%d & %dX%d with %d no. of threads is %f\n", ROW_A, COL_A, ROW_B, COL_B, numThreads, time_spent);
	fprintf (resultFilePointer, "***************************************************************************************\n");
}
