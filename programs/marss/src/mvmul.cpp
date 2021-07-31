#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
#include <omp.h>
typedef unsigned short datatype;

int main ( int argc, char* argv[] ) {


    // Random numbers
    int lower = 0;
    int upper = 65535;

    // Array dimensions
    int M = atoi(argv[1]); // input
    int N = atoi(argv[2]); // output

    // Create arrays that represent the matrices A,B,C
    datatype*  A = (datatype*) malloc (M*N * sizeof(datatype));
    datatype*  x = (datatype*) malloc (M * sizeof(datatype));
    datatype*  c = (datatype*) malloc (N * sizeof(datatype));

    // Fill A and B with random numbers
    for(int i = 0; i < M * N; i += 1)
      A[i] = (rand() % (upper - lower + 1)) + lower;
    for(int i = 0; i < M; i += 1)
      x[i] = (rand() % (upper - lower + 1)) + lower;
    for(int i = 0; i < N; i += 1)
      c[i] = (datatype) 0;


    // Checkpointing
    char * chk_name = getenv ("CHECKPOINT_NAME");
    if( chk_name != NULL ) {
	printf (" Creating checkpoint with name %s\n", chk_name ) ;
	ptlcall_checkpoint_and_shutdown ( chk_name );
    }


    // mkn algorithm
    omp_set_dynamic(0);
    omp_set_num_threads(32);
    #pragma omp parallel for
    for (int n = 0; n < N; n += 1) {
      for (int m = 0; m < M; m += 1) {
        c[n] += A[M*n + m] * x[m];
      }
    }

    ptlcall_kill();
}
