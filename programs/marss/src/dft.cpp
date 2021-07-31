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
    int trial = atoi(argv[1]);
    int M = atoi(argv[2]);
    int N = atoi(argv[3]);

    // Create arrays that represent the matrices A,B,C
    datatype*  Areal = (datatype*) malloc (M*N * sizeof(datatype));
    datatype*  Aimag = (datatype*) malloc (M*N * sizeof(datatype));
    datatype*  x = (datatype*) malloc (M * sizeof(datatype));
    datatype*  creal = (datatype*) malloc (N * sizeof(datatype));
    datatype*  cimag = (datatype*) malloc (N * sizeof(datatype));
    // Fill A and B with random numbers
    for(int i = 0; i < M * N; i += 1) {
      Areal[i] = (rand() % (upper - lower + 1)) + lower;
      Aimag[i] = (rand() % (upper - lower + 1)) + lower;
    }
    for(int i = 0; i < M; i += 1)
      x[i] = (rand() % (upper - lower + 1)) + lower;
    for(int i = 0; i < N; i += 1) {
      creal[i] = (datatype) 0;
      cimag[i] = (datatype) 0;
    }

    // Checkpointing
    char * chk_name = getenv ("CHECKPOINT_NAME");
    if( chk_name != NULL ) {
	printf (" Creating checkpoint with name %s\n", chk_name ) ;
	ptlcall_checkpoint_and_shutdown ( chk_name );
    }


    // mkn algorithm
    omp_set_dynamic(0);
    omp_set_num_threads(32);
    for (int inst = 0; inst < trial; inst += 1) {
	 #pragma omp parallel for
        for (int m = 0; m < M; m += 1) {
          for (int n = 0; n < N; n += 1) {
            creal[n] += Areal[M*n + m] * x[m];
            cimag[n] += Aimag[M*n + m] * x[m];
          }
        }
    }
    ptlcall_kill();
}
