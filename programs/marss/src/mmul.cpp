#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
int main ( int argc, char* argv[] ) {
    typedef unsigned short datatype;

    // Random numbers
    int lower = 0;
    int upper = 65535;

    // Array dimensions
    int M = atoi(argv[1]);
    int N = 256;
    int K = atoi(argv[2]);

    // Create arrays that represent the matrices A,B,C
    datatype*  A = (datatype*) malloc (M*K * sizeof(datatype));
    datatype*  B = (datatype*) malloc (K*N * sizeof(datatype));
    datatype*  C = (datatype*) malloc (M*N * sizeof(datatype));

    // Fill A and B with random numbers
    for(int i = 0; i < M * K; i += 1)
      A[i] = (rand() % (upper - lower + 1)) + lower;
    for(int i = 0; i < K * N; i += 1)
      B[i] = (rand() % (upper - lower + 1)) + lower;
    for(int i = 0; i < M * N; i += 1)
      C[i] = (datatype) 0;

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
    for (int m = 0; m < M; m += 1) {
      for (int k = 0; k < K; k += 1) {
        for (int n = 0; n < N; n += 1) {
          C[N*m + n] += A[K*m + k] * B[N*k + n];
        }
      }
    }
    ptlcall_kill();
}
