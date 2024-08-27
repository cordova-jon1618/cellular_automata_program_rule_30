
/* ==================================
Name: cellular_automata_sequential.c
Author: Jonathan Cordova
Date: 04/27/2022
Course: COMP620 - Spring2022 - ClassNumber 12389
====================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <math.h>


//-------------------------------------------------
// PRINTING FUNCTIONS
//-------------------------------------------------


void printVertex(int* arr, int n) {
	printf("\t");
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
		if ((i + 1) % n == 0) {
			printf("\n");
		}//end if
	}//end for
	printf("\n\n");
}//end printVertex



void printAsPic(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		if(arr[i] == 0){
			printf(". ");
		}
		if(arr[i] == 1){
			printf("* ");
		}
		if ((i + 1) % n == 0) {
			printf("\n");
		}//end if
	}//end for
	printf("\n\n");
}//end printAsPic


// print Matrix m by n
void printMatrix(int *arr, int m, int n){
    int i, j;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
           printf("%3d ", arr[i*n+j]);
       }// end inner for
        printf("\n");
    }// end outer for
    printf("\n");
} // end printMatrix

//-------------------------------------------------
// CA RULE FUNCTIONS
//-------------------------------------------------

int rule30check(int x1, int x2, int x3){
	int nextgen = -1;

	if (x1 == 0 && x2 == 0 && x3 == 0){ //0
		nextgen = 0;
	}else if(x1 == 0 && x2 == 0 && x3 == 1){ //1
		nextgen = 1;
	}else if(x1 == 0 && x2 == 1 && x3 == 0){ //2
		nextgen = 1;
	}else if(x1 == 0 && x2 == 1 && x3 == 1){ //3
		nextgen = 1;
	}else if(x1 == 1 && x2 == 0 && x3 == 0){ //4
		nextgen = 1;
	}else if(x1 == 1 && x2 == 0 && x3 == 1){ //5
		nextgen = 0;
	}else if(x1 == 1 && x2 == 1 && x3 == 0){ //6
		nextgen = 0;
	}else if(x1 == 1 && x2 == 1 && x3 == 1){ //7
		nextgen = 0;
	}

	return nextgen;

} // end rule30check




//-------------------------------------------------
// MAIN FUNCTION
//-------------------------------------------------

int main(int argc, char* argv[]) {

	int n = 50;
	int rows = n;
	int cols = n;
	int ind1 = 0;
	int ind2 = 0;
	int ind3 = 0;

	int mid = n / 2;

	int* a = malloc((n * n) * sizeof(int));


    // Initialize a[] vertex with 0's.
    for(int i = 0; i <= n; i++){
        a[i] = 0;
    }
	a[mid] = 1;

	//printMatrix(a, rows, cols);


//-------------------------------------------------
// ELEMENTARY CELLULAR AUTOMATON ALGORITHM
//-------------------------------------------------

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(i > 0 && i < (n - 1)){
				ind1 = (i-1) * cols + (j-1);
				ind2 = (i-1) * cols + j;
				ind3 = (i-1) * cols + (j+1);
				a[i * cols + j] = rule30check(a[ind1], a[ind2], a[ind3]);
			}
		}
	}


	printf("\n-------------------------------------------Output.-------------------------------------------------\n\n");

	//Print output
	//printMatrix(a, rows, cols);
	//printVertex(a, (n*n));
	printAsPic(a, n*n);

	
	printf("\n----------------------------------------------------------------------------------------------------\n\n");


	free(a);

	return 0;

}//end main
