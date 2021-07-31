#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
#include <omp.h>
#include <cstdlib>
#include <algorithm> 

typedef char datatype;

int main ( int argc, char* argv[] ) {


    // Random numbers
    int lower = 0;
    int upper = 65535;

    int M = atoi(argv[1]);
    int pattern_length = atoi(argv[2]);

    // Create arrays that represent the matrices A,B,C
    datatype*  strings = (datatype*) malloc (M * 512 * sizeof(datatype));
    datatype* pattern  = (datatype*) malloc ( pattern_length  * sizeof(datatype));
    bool* matches = (bool*) malloc (M * (512 -  pattern_length ) * sizeof(bool));

    // Fill A and B with random numbers
    for(int i = 0; i < M * 512; i += 1) {
      strings[i] = (rand() % (upper - lower + 1)) + lower;
    }
    for(int i = 0; i <  pattern_length ; i += 1) {
      pattern[i] = (rand() % (upper - lower + 1)) + lower;
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
    for (int thread = 0; thread < M; thread += 1) {
	datatype * substring = &strings[thread*512];
	bool* match = &matches[thread * (512 -  pattern_length )];
	for (int i = 0; i < 512 -  pattern_length ; i += 1) {
		match[i] = (bool) 1;
		for (int j =0; j < pattern_length; j += 1) {
		   matches[i] &= substring[i + j] == pattern[j];
		}
	}
    }
    ptlcall_kill();
}
