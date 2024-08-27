/* ==================================
Name: cellular_automata_parallel.c
Author: Jonathan Cordova
Date: 04/27/2022
Course: COMP620 - Spring2022 - ClassNumber 12389
Description: Write an efficient parallel c program using p (p <= 10) processors to solve one-dimensonal
            heat distribution problem with n (n >= 1000) elements based upon finite difference equation.  Assume n
            is divisible by p and n >> p. a[i] = .25 * (a[i-1] + 2*a[i] + a[i+1]).
To Compile: mpicc cellular_automata_parallel.c
To Execute: mpiexec -n p ./a.out n f1 f2 f3
Example:    mpiexec -n 10 ./a.out 1000 150.0 75.0 150.0
Please Note: p must be divisible by n, otherwise
			 data will be lost.
====================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#define mpc MPI_COMM_WORLD


//-------------------------------------------------
// PRINTING FUNCTIONS
//-------------------------------------------------


void printVertex(float* arr, int n) {
	printf("\t");
	for (int i = 0; i < n; i++) {
		printf("%.2f ", arr[i]);
		if ((i + 1) % 10 == 0) {
			printf("\n");
		}//end if
	}//end for
	printf("\n\n");
}//end printVertex



//-------------------------------------------------
// MAIN FUNCTION
//-------------------------------------------------

int main(int argc, char* argv[]) {

	
	int err, procs, id, n, k;
    float f1, f2, f3;
	MPI_Status status;
	// initialize MPI_Init  
	err = MPI_Init(&argc, &argv);
	if (err != MPI_SUCCESS) {
		printf("\nError initializing MPI.\n");
		MPI_Abort(mpc, err);}
	// get no. of processes	
	MPI_Comm_size(mpc, &procs);
	// get processor id
	MPI_Comm_rank(mpc, &id);
	// to print only once....
	if (id == 0 && argc < 2) {
		printf("\n\tHello, You have entered an invalid number of arguments."
			"\n\tThis Parallel C Program solved a one-dimensonal heat distribution problem with n (n >= 1000)"
			"\n\telements based upon finite difference equation. Assume n is divisible by p and n >> p."
            "\n\tFormula: a[i] = .25 * (a[i-1] + 2*a[i] + a[i+1])"
			"\n\tTo Compile: mpicc cellular_automata_parallel.c"
			"\n\tTo Execute: mpiexec -n p ./a.out n f1 f2 f3"
            "\n\tExample: mpiexec -n 10 ./a.out 1000 150.0 75.0 150.0"
			"\n\tNote: Processor total (procs) must be divisible by n, otherwise data will be lost.\n\n");
	}

	// Terminate MPI if argc < 2
	if (argc < 2) { MPI_Finalize(); return 0; } // end if
	// get n
	n = atoi(argv[1]);
    f1 = atof(argv[2]);
    f2 = atof(argv[3]);
    f3 = atof(argv[4]);

	k = n / procs;

	if(n % procs != 0){
		printf("\n\tProcessors (procs) must be divisible by n, otherwise data will be lost.\n");
	}

    int mid;
	int startindex = 0;
    int iterationlimit = n * k;
	int count = 0;
	int flag = 0;

	mid = n / 2;

	float* a = malloc((n + 2) * sizeof(float));
	float* b = malloc((n + 2) * sizeof(float));
	float* finalarr = malloc((n + 2) * sizeof(float));

    //initialize borders and array
    a[0] = f1; 
    a[n+1] = f3;

    // Initialize a[] vertex with 0.0's.
    for(int i = 1; i <= n; i++){
        a[i] = 0.0;
    }


//--------------------------------------------------------
// MASTER PROCESSOR
//--------------------------------------------------------
	if (id == 0){

        //printVertex(a, n+2);

    }//end Master

//-------------------------------------------------
// ALL PROCESSORS - HEAT DISTRIBUTION ALGORITHM
//-------------------------------------------------

    MPI_Bcast(a, n + 2, MPI_FLOAT, 0, mpc);
    MPI_Bcast(b, n + 2, MPI_FLOAT, 0, mpc);


    // Each processor gets k rows
    for(int iteration = 0; iteration < iterationlimit; iteration++){

		startindex = id * k;
	
    	for(int i = (startindex + 1); i <= (startindex + k); i++){
				b[i] = .25 * (a[i - 1] + 2 * a[i] + a[i + 1]);
				a[i] = b[i];
		}


		//-------------------------------------------------
		// EVENS SEND, ODDS RECEIVE 
		//-------------------------------------------------


		//EVEN and MASTER
		if (id == 0){

			MPI_Send(&b[startindex + k], 1, MPI_FLOAT, id + 1, id, mpc);
			
		}

		//EVEN but NOT MASTER Nor LAST PROCESSOR
		else if(id % 2 == 0 && id < (procs - 1)){

			MPI_Send(&b[startindex + k], 1, MPI_FLOAT, id + 1, id, mpc);
			MPI_Send(&b[startindex + 1], 1, MPI_FLOAT, id - 1, id, mpc);

		}
		//ODD but NOT LAST PROCESSOR
		else if(id % 2 != 0 && id < (procs - 1)){

			MPI_Recv(&a[startindex], 1, MPI_FLOAT, id - 1, id - 1, mpc, &status);
			MPI_Recv(&a[startindex + k + 1], 1, MPI_FLOAT, id + 1, id + 1, mpc, &status);

		}
		//ODD and LAST PROCESSOR
		else if (id % 2 != 0 && id == (procs - 1)) {

			MPI_Recv(&a[startindex], 1, MPI_FLOAT, id - 1, id - 1, mpc, &status);

		}
		//EVEN and LAST PROCESSOR
		else if (id % 2 == 0 && id == (procs - 1)) {

			MPI_Send(&b[startindex + 1], 1, MPI_FLOAT, id - 1, id, mpc);

		}


		//-------------------------------------------------
		// EVENS RECEIVE, ODDS SEND
		//-------------------------------------------------

		
		//EVEN and MASTER
		if (id == 0){

			MPI_Recv(&a[startindex + k + 1], 1, MPI_FLOAT, id + 1, id + 1, mpc, &status);
		}

		//EVEN but NOT MASTER Nor LAST PROCESSOR
		else if(id % 2 == 0 && id < (procs - 1)){

			MPI_Recv(&a[startindex + k + 1], 1, MPI_FLOAT, id + 1, id + 1, mpc, &status);
			MPI_Recv(&a[startindex], 1, MPI_FLOAT, id - 1, id - 1, mpc, &status);

		}
		//ODD but NOT LAST PROCESSOR
		else if(id % 2 != 0 && id < (procs - 1)){

			MPI_Send(&b[startindex + 1], 1, MPI_FLOAT, id - 1, id, mpc);
			MPI_Send(&b[startindex + k], 1, MPI_FLOAT, id + 1, id, mpc);
		}
		//ODD and LAST PROCESSOR
		else if(id % 2 != 0 && id == (procs - 1)){

			MPI_Send(&b[startindex + 1], 1, MPI_FLOAT, id - 1, id, mpc);

		}
		//EVEN and LAST PROCESSOR
		else if (id % 2 == 0 && id == (procs - 1)) {

			MPI_Recv(&a[startindex], 1, MPI_FLOAT, id - 1, id - 1, mpc, &status);

		}


		for(int i = startindex + 1; i <= (startindex + k); i++){
				b[i] = .25 * (a[i - 1] + 2 * a[i] + a[i + 1]);
				a[i] = b[i];
		}
		

		//End iterations early if a[mid] > f2.
		if(a[mid] > f2){
			flag = 1;
			MPI_Bcast(&flag, 1, MPI_INT, id, mpc);
		}

		if(flag == 1){
			break;
		}


	}//end iteration

MPI_Barrier(mpc);

//--------------------------------------------------------
// SUB PROCESSOR
//--------------------------------------------------------
if(id > 0){

	//All Sub Processors send to Master Processor to create final array
	MPI_Send(a + ((id * k) + 1), k, MPI_FLOAT, 0, id, mpc);
}

//--------------------------------------------------------
// MASTER PROCESSOR
//--------------------------------------------------------
	if (id == 0){

	 //initialize borders of final array
    finalarr[0] = f1; 
    finalarr[n+1] = f3;

    // Master's local section
    for(int i = 1; i <= k; i++){
        finalarr[i] = a[i];
    }


	//Receiving from all processors
	for(int i = 1; i < procs; i++){
		MPI_Recv(finalarr + ((i * k) + 1), k, MPI_FLOAT, i, i, mpc, &status);
	}

	//Final Output
	printf("\n\tParallel Processing of the Heat Distribution Algorithm by Jonathan Cordova.\n\n");
	printf("\n\t-------------------------------Final Output------------------------------\n");
	printf("\n\tfinalarr[]:\n");
    printVertex(finalarr, n+2);
	printf("\n\t-------------------------------------------------------------------------\n\n");
 
	} //end Master



	free(a);
	free(b);
	free(finalarr);

	MPI_Finalize();
	return 0;

}//end main
