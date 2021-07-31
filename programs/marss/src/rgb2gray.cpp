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
    datatype*  in =  (datatype*) malloc (inst*3*128*128* sizeof(datatype));
    datatype*  out = (datatype*) malloc (inst*128*128* sizeof(datatype));

    // Fill A and B with random numbers 
    short lumR = 2;
    short lumG = 3;
    short lumB = 4;
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
       int o = 0;
       for (int i = 0; i < 128*128*3; i += 3) {
	   short tmp;
           short r = in [img*128*128*3 + i];
	   short g = in [img*128*128*3 + i + 1];
	   short b = in [img*128*128*3 + i + 2];
 	   o += 1;
	   out [img*128*128 + o] = lumR * r + lumG * b + lumB * b;
        }
    }
    ptlcall_kill();
}
