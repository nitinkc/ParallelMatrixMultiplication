
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

//Random Number Range
#define MAXRAND 9999

//Dimensions of the Matrices to be multiplied
#define ROW_A 500
#define COL_A 200
#define ROW_B 200
#define COL_B 250

/* GLOBAL VARIABLE DECLAARTION */

	/* Pointer for the three Matrix*/
		double *matA=NULL;
		double *matB=NULL;
		double *matC=NULL;


	/* Variables for Parallel execution time*/
		double begin=0, end=0;
		double time_spent=0;

	/* Declare number of Threads for Parallel Execution*/
		int noThreads=4;
		int noRows, threadId;

	/* Function Prototypes */
		void allocateMemory();
		void fillMatrix();
		void collectResults();

/* MAIN BEGINS */
int main(){


	/* TEMP : WELCOME MESSAGE AND ASKING FOR USER DEFINED NUMDER OF THREADS*/
	printf("=============================WELCOME============================\n");
	printf("                             TESTING                         \n");
	printf("================================================================\n\n");


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

		/* For parallel threaded multiplication, Rows from matrix A is to be multiplied with Columns
		 * of matrix B resulting in the Rows of Matrix C. Thus each thread can work on some Rows,
		 * depending upon the division of jobs into threads  */
	int i,j,k;
		//noThreads = omp_get_num_threads();
		//printf("Number of threads = %d\n", noThreads);
		noRows = ROW_A/noThreads;
		omp_set_num_threads(noThreads); //set the number of threads
		printf("Parallel Threads running = %d\n", noRows);
		begin = omp_get_wtime();
		#pragma omp parallel shared(matA,matB,matC,noRows) private(threadId)
		{

			//Split the first for loop among the threads
			#pragma omp for schedule(dynamic,noRows)
			//#pragma omp for
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


void collectResults(){

	/* Variable Declaration*/
	FILE *resultFilePointer;
	resultFilePointer = fopen ("Results.csv","a+"); //Append mode, Returns the File descriptor (Null pointer otherwise)
	if (resultFilePointer == NULL) {
		printf ("Cannot open file to write!\n");
		 exit(-1);
	}
	
	//For Current System time
	time_t mytime;
	mytime = time(NULL);
	printf("TESTING : Writing Parallel Multiplication data in file\n");
	fprintf(resultFilePointer,"Testing done on : %s", ctime(&mytime));
	fprintf (resultFilePointer, "Parallel execution time of Matrices of dim %dX%d & %dX%d with %d no. of threads is %f\n", ROW_A, COL_A, ROW_B, COL_B, noThreads, time_spent);
	fprintf (resultFilePointer, "***************************************************************************************\n");


}
