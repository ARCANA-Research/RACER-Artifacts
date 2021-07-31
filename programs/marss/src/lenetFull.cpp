
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>
#include "/home/alex/simulator/marss.dramsim/ptlsim/tools/ptlcalls.h"

uint8_t max(uint8_t a, uint8_t b)
{
	return a >= b ? a : b;
}


void mvmul(uint8_t* Mat, uint8_t* vec, uint8_t M, uint8_t N, uint8_t* out)
{
    omp_set_dynamic(0);
    omp_set_num_threads(32);
    #pragma omp parallel for
    for (int n = 0; n < N; n += 1) {
      for (int m = 0; m < M; m += 1) {
        out[n] += Mat[M*n + m] * vec[m];
      }
    }
}

void mvmulForPool(uint8_t* Mat, uint8_t* vec, int M, int N, uint8_t* out)
{
    omp_set_dynamic(0);
    omp_set_num_threads(32);
    #pragma omp parallel for
    for (int n = 0; n < N; n += 1) {
      for (int m = 0; m < M; m += 1) {
        out[n] += max(Mat[M*n + m] & vec[m], out[n]);
      }
    }
}

void relu(uint8_t* in, uint8_t* out, int size)
{
    omp_set_dynamic(0);
    omp_set_num_threads(32);
    #pragma omp parallel for
    for (int n = 0; n < size; n += 1) {
	out[n] = in[n] > 0 ? in[n] : 0;
    }
}

void layer1(uint8_t* weightConv, uint8_t* weightPool, uint8_t* in, uint8_t* imm, uint8_t* out)
{
	for (int outchannel = 0; outchannel < 6; outchannel+=1) {
		mvmul(&weightConv[outchannel*1024*784], in, 1024, 784, &imm[outchannel*784]);
		mvmulForPool(&weightPool[outchannel*784*196], &imm[outchannel*784], 784, 196, &out[outchannel*196]);
	}
	relu(out, out, 196*6);
}

void layer2(uint8_t* weightConv, uint8_t* weightPool, uint8_t* in, uint8_t* imm0, uint8_t* imm1, uint8_t* out)
{
	for (int outchannel = 0; outchannel < 16; outchannel+=1) {
		mvmul(&weightConv[outchannel*196*100*6], in, 196, 100*6, &imm0[outchannel*100*6]);
		#pragma omp parallel for
		for (int i = 0; i < 100; i+=1) {
			for (int j = 0; j < 6; j+=1) {
				//printf("%d\n",i);
				imm1[outchannel*100 + i] += imm0[outchannel*100*6 + i*6 + j];
			}
		}
		mvmulForPool(&weightPool[outchannel*100*25], &imm1[outchannel*100], 100, 25, &out[outchannel*25]);
	}
	relu(out, out, 25*16);
}

void layer3(uint8_t* weightConv, uint8_t* in, uint8_t* imm, uint8_t* out)
{
	for (int outchannel = 0; outchannel < 120; outchannel+=1) {
		mvmul(&weightConv[outchannel*25*1*16], in, 25, 1*16, &imm[outchannel*1*16]);
		#pragma omp parallel for
		for (int i = 0; i < 1; i+=1) {
			for (int j = 0; j < 16; j+=1) {
				//printf("%d\n",i);
				out[outchannel*1 + i] += imm[outchannel*1*16 + i*16 + j];
			}
		}
	}
}


int main(int argc, char const *argv[]) {
  int lower = 0;
  int upper = 65535;
	int I = atoi(argv[1]);

	// L1 Allocation
	uint8_t* L1weightConv = (uint8_t*) malloc(1024*784*1*6*sizeof(uint8_t));
	uint8_t* L1weightPool = (uint8_t*) malloc(784*196*6*sizeof(uint8_t));
	uint8_t* L1in         = (uint8_t*) malloc(1024*1*I*sizeof(uint8_t));
	uint8_t* L1imm        = (uint8_t*) malloc(784*6*sizeof(uint8_t));
	uint8_t* L1out				= (uint8_t*) malloc(196*6*sizeof(uint8_t));

	for(int i = 0; i < 1024*784*1*6; i+=1)
		L1weightConv[i] = (rand() % (upper - lower + 1)) + lower;
	for(int i = 0; i < 784*196*6; i+=1)
		L1weightPool[i] = (rand() % (upper - lower + 1)) + lower;
	for(int i = 0; i < 1024*1; i+=1)
		L1in[i] = (rand() % (upper - lower + 1)) + lower;
	for(int i = 0; i < 784*6; i+=1)
		L1imm[i] = (uint8_t) 0;
	for(int i = 0; i < 196*6; i+=1)
		L1out[i] = (uint8_t) 0;

	// L2 Allocation
	uint8_t* L2weightConv = (uint8_t*) malloc(196*100*6*16*sizeof(uint8_t));
	uint8_t* L2weightPool = (uint8_t*) malloc(100*25*16*sizeof(uint8_t));
	uint8_t* L2imm0        = (uint8_t*) malloc(100*6*16*sizeof(uint8_t));
	uint8_t* L2imm1        = (uint8_t*) malloc(100*16*sizeof(uint8_t));
	uint8_t* L2out				= (uint8_t*) malloc(25*16*sizeof(uint8_t));

	for(int i = 0; i < 196*100*6*16; i+=1)
		L2weightConv[i] = (rand() % (upper - lower + 1)) + lower;
	for(int i = 0; i < 100*25*16; i+=1)
		L2weightPool[i] = (rand() % (upper - lower + 1)) + lower;
	for(int i = 0; i < 100*6*16; i+=1)
		L2imm0[i] = (uint8_t) 0;
	for(int i = 0; i < 100*16; i+=1)
		L2imm1[i] = (uint8_t) 0;
	for(int i = 0; i < 25*16; i+=1)
		L2out[i] = (uint8_t) 0;

	// L3 Allocation
	uint8_t* L3weightConv = (uint8_t*) malloc(25*1*16*120*sizeof(uint8_t));
	uint8_t* L3imm        = (uint8_t*) malloc(1*16*120*sizeof(uint8_t));
	uint8_t* L3out        = (uint8_t*) malloc(1*120*sizeof(uint8_t));

	for(int i = 0; i < 25*1*16*120; i+=1)
		L3weightConv[i] = (rand() % (upper - lower + 1)) + lower;
	for(int i = 0; i < 1*16*120; i+=1)
		L3imm[i] = (uint8_t) 0;
	for(int i = 0; i < 1*120; i+=1)
		L3out[i] = (uint8_t) 0;

	// FC Allocation
	uint8_t* FC1Mat = (uint8_t*) malloc(120*84*sizeof(uint8_t));
	uint8_t* FC1out = (uint8_t*) malloc(84*sizeof(uint8_t));
	uint8_t* FC2Mat = (uint8_t*) malloc(84*10*sizeof(uint8_t));
	//uint8_t* FC2out = (uint8_t*) malloc(10*sizeof(uint8_t));

	uint8_t* decision = (uint8_t*) malloc(10*I*sizeof(uint8_t));

	//double time = omp_get_wtime();

    char* chk_name = getenv("CHECKPOINT_NAME") ;
    if(chk_name != NULL) {
      printf (" Creating checkpoint with name %s\n", chk_name ) ;
      ptlcall_checkpoint_and_shutdown ( chk_name );
    }
	for (int i = 0; i < I; i+=1) {
		uint8_t* img = &L1in[i * 1024];
		layer1(L1weightConv, L1weightPool, img, L1imm, L1out);
		layer2(L2weightConv, L2weightPool, L1out, L2imm0, L2imm1, L2out);
		layer3(L3weightConv, L2out, L3imm, L3out);
		mvmul(FC1Mat, L3out, 120, 84, FC1out);
		mvmul(FC2Mat, FC1out, 84, 10, &decision[i*10]);
	}
	//time = omp_get_wtime() - time;
	//printf("%f seconds\n", time);

    ptlcall_kill();
	return 0;
}

