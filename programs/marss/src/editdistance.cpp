// A Naive recursive C++ program to find minimum number
// operations to convert str1 to str2
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
#include <omp.h>
#include <cstdlib>
#include <string.h>
using namespace std;

// Utility function to find minimum of three numbers
int min(int x, int y, int z) { return min(min(x, y), z); }


int editDistDP(char* str1, char* str2, int len1, int len2)
{
   // Create a DP array to memoize result
   // of previous computations
   int DP[2][len1 + 1];

   // To fill the DP array with 0
   memset(DP, 0, sizeof DP);

   // Base condition when second string
   // is empty then we remove all characters
   for (int i = 0; i <= len1; i++)
       DP[0][i] = i;

   // Start filling the DP
   // This loop run for every
   // character in second string
   for (int i = 1; i <= len2; i++) {
       // This loop compares the char from
       // second string with first string
       // characters
       for (int j = 0; j <= len1; j++) {
           // if first string is empty then
           // we have to perform add character
           // operation to get second string
           if (j == 0)
               DP[i % 2][j] = i;

           // if character from both string
           // is same then we do not perform any
           // operation . here i % 2 is for bound
           // the row number.
           else if (str1[j - 1] == str2[i - 1]) {
               DP[i % 2][j] = DP[(i - 1) % 2][j - 1];
           }

           // if character from both string is
           // not same then we take the minimum
           // from three specified operation
           else {
               DP[i % 2][j] = 1 + min(DP[(i - 1) % 2][j],
                                      min(DP[i % 2][j - 1],
                                          DP[(i - 1) % 2][j - 1]));
           }
       }
   }

   // after complete fill the DP array
   // if the len2 is even then we end
   // up in the 0th row else we end up
   // in the 1th row so we take len2 % 2
   // to get row
   return DP[len2 % 2][len1];
}

// Driver code
int main( int argc, char* argv[] )
{
    // Random numbers
    int lower = 0;
    int upper = 65535;
    int a = 4096;
    int b = 512;

    // Array dimensions
    int M = atoi(argv[1]);
    // Create arrays that represent the matrices A,B,C
    char* As = (char*) malloc (M * a * sizeof(char));
    char* Bs = (char*) malloc (M * b * sizeof(char));
    int* distances = (int*) malloc (M * sizeof(int));

    // Fill A and B with random numbers
    for(int i = 0; i < M * a; i += 1) {
      Bs[i] = (rand() % (upper - lower + 1)) + lower;
    }
    for(int i = 0; i < M * b; i += 1) {
      As[i] = (rand() % (upper - lower + 1)) + lower;
    }

  //   // Checkpointing
    char * chk_name = getenv ("CHECKPOINT_NAME");
    if( chk_name != NULL ) {
	 printf (" Creating checkpoint with name %s\n", chk_name ) ;
	 ptlcall_checkpoint_and_shutdown ( chk_name );
     }


    // mkn algorithm
    omp_set_dynamic(0);
    omp_set_num_threads(16);
    #pragma omp parallel for
    for (int i = 0; i < M; i += 1) {
  	    char* A = &As[i*a];
      	char* B = &Bs[i*b];
        distances[i] = editDistDP(A, B, a, b);
    }
    ptlcall_kill();
    return 0;
}
