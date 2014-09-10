/* parallelMatMult.c
 * The program generates 2 matrices of dimensions 5000X2000 and 2000X2500 with
 * all elements as random generated double values
 *
 * The multiplication is done in a parallel fashion and it is compared with that
 * of the sequential one.
 *
 * Author: Nitin K Chaurasia
 *
 *  Created on: Sep 10, 2014, 1:17 AM
 *  Modified: Sep 10, 2014
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#define MAXRAND 9999

#define ROW_A 5000
#define COL_A 2000
#define ROW_B 2000
#define COL_B 2500

/* Global Variable Declaration*/

	/* Pointer for the three Matrix*/
	double *matA=NULL;
	double *matB=NULL;
	double *matC=NULL;

void allocateMemory();

int main(){

	/* Variable Declaration*/
	int i,j,k;



	/* Declare number of Threads */
	int noThreads = 1;
	int noRows, threadId;
	/* Variables for execution time*/
	double begin, end;
	double time_spent;

	/* TEMP : WELCOME MESSAGE AND ASKING FOR USER DEFINED NUMDER OF THREADS*/
	printf("=============================WELCOME============================\n");
	printf("LAB ASSIGNMENT 1\n");
	printf("================================================================\n\n");
	printf("CHECK IF USER DEFINED NUMBER OF THREADS CAN BE CONTROLLED\n");
	printf("Number of Threads: ");
	scanf("%d", &noThreads);
	printf("The number of threads are %d\n", noThreads);

	/* Check for multiplication compatibility */
		if (COL_A != ROW_B){
			printf("Column for Matrix A should be same as that of Row for Matrix B\n");
			printf("Multiplication is not possible\n");
			return(0);
		}

		/* Allocate Memory for the Matrix and add Random Double values into it */
		allocateMemory();

	/* For parallel threaded multiplication, Rows from matrix A is to be multiplied with Columns
	 * of matrix B resulting in the Rows of Matrix C. Thus each thread can work on some Rows,
	 * depending upon the division of jobs into threads  */

	noRows = ROW_A/noThreads;
	omp_set_num_threads(noThreads); //set the number of threads
	printf("Parallel Threads running = %d", noRows);
	begin = omp_get_wtime();
	#pragma omp parallel shared(matA,matB,matC,noRows) private(threadId)
	{
		threadId = omp_get_thread_num(); //holds the thread number of each thread

		//Split the first for loop among the threads
		#pragma omp for schedule(guided,noRows)
		//Multiplication of 2 Matrices using traditional 3 loop Algorithm
		  for(i=0;i<ROW_A;i++){ //row of first matrix
			  //printf("Thread #%d is working on row %d.\n",threadId,i);
			  for(j=0;j<COL_B;j++){  //column of second matrix
				  for(k=0;k<COL_A;k++){
					  *( matC+(i*COL_A+j) ) += *( matA+(i*ROW_A+k) )*( *( matB+(k*COL_B+j) ));
				}//end k
			  }//end j
		  }//end i
	  }
	end = omp_get_wtime();
	time_spent = (double)(end - begin) ;
	printf("The time spent is : %1.5f\n", time_spent);

		//free all of the memory
	printf("Freeing memory.....\n");
	  free(matA);
	  free(matB);
	  free(matC);

	/* Output FILE HANDLING */
	FILE *resultFilePointer;
	resultFilePointer = fopen ("Results.csv","a+"); //Append mode, Returns the File descriptor (Null pointer otherwise)
	if (resultFilePointer == NULL) {
		printf ("Cannot open file to write!\n");
		 exit(-1);
	}
	fprintf (resultFilePointer, "Parallel execution time of Matrices of dim %dX%d & %dX%d with %d no. of threads is %f\n", ROW_A, COL_A, ROW_B, COL_B, noThreads, time_spent);

  return 0;
} //end main()

void allocateMemory(){
	int i,j,k;
	/* If Multiplication is Possible, allocate memory for the three matrices and check if its failed */
			 matA = malloc(ROW_A*COL_A*sizeof(double));
			  if(matA==NULL)
				  printf("\nMemory allcation failed for matrix A.\n");
			  matB = malloc(ROW_B*COL_B*sizeof(double));
			  if(matB==NULL)
				  printf("\nMemory allcation failed for matrix B.\n");
			  matC = malloc(ROW_A*COL_B*sizeof(double));
			  if(matC==NULL)
				  printf("\nMemory allcation failed for matrix C.\n");


		/* Initialize the seed to generate Random Values */
		srand((unsigned)time(NULL));

		/* Generate Matrices */
		//Matrix A
		for(i=0;i<ROW_A;i++){
			for(j=0;j<COL_A;j++){
				//matA[i][j]=((double)rand()/(double)MAXRAND);//traditional method of fixed size array
				*(matA+(i*COL_A+j))=((double)rand()/(double)MAXRAND);
			}//End Loop for Column
		}//End Loop for Row

		//Matrix B
		for(i=0;i<ROW_B;i++){
			for(j=0;j<COL_B;j++){
				//matB[i][j]=((double)rand()/(double)MAXRAND);
				*(matB+(i*COL_A+j))=((double)rand()/(double)MAXRAND);
			}//End Loop for Column
		}//End Loop for Row

		//Matrix C (initialized to Zero)
		for(i=0;i<ROW_A;i++){
			for(j=0;j<COL_B;j++){
				*(matC+(i*COL_B+j)) = 0;
			}//End Loop for Column
		}//End Loop for Row
}
