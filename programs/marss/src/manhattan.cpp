#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
#include <omp.h>
#include <cstdlib>

using namespace std;
typedef unsigned short datatype;

int main ( int argc, char* argv[] ) {


    // Random numbers
    int lower = 0;
    int upper = 65535;

    // Array dimensions
    int M = atoi(argv[1]); // input

    // Create arrays that represent the matrices A,B,C
    datatype*  databaseX = (datatype*) malloc (M * sizeof(datatype));
    datatype*  databaseY = (datatype*) malloc (M * sizeof(datatype));
    datatype*  databaseZ = (datatype*) malloc (M * sizeof(datatype));
    datatype*  pointsX   = (datatype*) malloc (64 * sizeof(datatype));
    datatype*  pointsY   = (datatype*) malloc (64 * sizeof(datatype));
    datatype*  pointsZ   = (datatype*) malloc (64 * sizeof(datatype));
    datatype*  distance = (datatype*) malloc (M * 64 * sizeof(datatype));

    // Fill A and B with random numbers
    for(int i = 0; i < M; i += 1) {
      databaseX[i] = (rand() % (upper - lower + 1)) + lower;
      databaseY[i] = (rand() % (upper - lower + 1)) + lower;
      databaseZ[i] = (rand() % (upper - lower + 1)) + lower;
    }

    for(int i = 0; i < 64; i += 1) {
      pointsX[i] = (rand() % (upper - lower + 1)) + lower;
      pointsY[i] = (rand() % (upper - lower + 1)) + lower;
      pointsZ[i] = (rand() % (upper - lower + 1)) + lower;
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
    #pragma omp parallel for
    for (int p = 0; p < 64; p += 1) {
	for (int m = 0; m < M; m+=1) {
		distance[p*M + m] = abs(pointsX[p] - databaseX[m]) +  abs(pointsY[p] - databaseY[m]) +  abs(pointsZ[p] - databaseZ[m]);
	}
    }
    ptlcall_kill();
}
