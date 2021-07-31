#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
#include <omp.h>
#include <cstdlib>

using namespace std;
typedef char datatype;

int main ( int argc, char* argv[] ) {


    // Random numbers
    int lower = 0;
    int upper = 65535;

    // Array dimensions
    int numFile = atoi(argv[1]);
    int M = atoi(argv[2]); // input

    // Create arrays that represent the matrices A,B,C
    datatype*  files = (datatype*) malloc (M * numFile * sizeof(datatype));
    int*  count = (int*) malloc (numFile * sizeof(int));
    datatype space = ' ';

    // Fill A and B with random numbers
    for(int i = 0; i < M*numFile; i += 1) {
      files[i] = (rand() % (upper - lower + 1)) + lower;
    }

    // Checkpointing
    char * chk_name = getenv ("CHECKPOINT_NAME");
    if( chk_name != NULL ) {
	printf (" Creating checkpoint with name %s\n", chk_name ) ;
	ptlcall_checkpoint_and_shutdown ( chk_name );
    }


    // mkn algorithm
    int global_count;
    omp_set_dynamic(0);
    omp_set_num_threads(32);
    #pragma omp parallel for
    for (int inst = 0; inst < numFile; inst += 1) {
	datatype* file = &files[inst * M];
	count[inst] = 0;
	for (int i = 0; i < M; i+=1) {
		count[inst] += space == file[i];
	}
    }
    ptlcall_kill();
}
