#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "/home/minh/git/marss_dramsim/marss.dramsim/ptlsim/tools/ptlcalls.h"
#include <omp.h>
typedef unsigned short datatype;

int main ( int argc, char* argv[] ) {


    // Random numbers
    int lower = 0;
    int upper = 255;

    // Array dimensions
    int inst = atoi(argv[1]); // number of images

    // Create arrays that represent the matrices A,B,C
    datatype*  in =  (datatype*) malloc (inst*128*128* sizeof(datatype));
    datatype*  out = (datatype*) malloc (inst*128*128* sizeof(datatype));

    // Fill A and B with random numbers 
    for(int i = 0; i < inst*128*128; i += 1) {
      in[i]  = (rand() % (upper - lower + 1)) + lower;
      out[i] = (rand() % (upper - lower + 1)) + lower;
    }

    // Checkpointing
    char * chk_name = getenv ("CHECKPOINT_NAME");
    if( chk_name != NULL ) {
	printf (" Creating checkpoint with name %s\n", chk_name ) ;
	ptlcall_checkpoint_and_shutdown ( chk_name );
    }

    omp_set_dynamic(0);
    omp_set_num_threads(32);
    #pragma omp parallel for
    for (int img = 0; img < inst; img+=1) {
       for (int i = 0; i < 128*128; i += 1) {
	   short tmp = 0;
	   tmp = in [img*128*128 + i] + 128;
	   if (tmp >= 256) {
		out[img*128*128 + i] = 255;
           }
	   else if (tmp < 0) {
	   	out[img*128*128 + i] = 0;
	   }
	   else {
		out[img*128*128 + i] = tmp;
	   }
        }
    }
    ptlcall_kill();
}
