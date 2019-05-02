/* Reed-Solomon encoder
 * Copyright 2002, Phil Karn, KA9Q
 * May be used under the terms of the GNU General Public License (GPL)
 */
#include <string.h>
#include "rs_encode.h"
#include "debug.h"
void encode_rs_8(data_t *data, data_t *parity,int pad){
  /*
  //Debugging print out
  printf("Debugging in C\n");
  printf("Pad = %d\n",pad);
  // hexdump(data,NN - NROOTS - pad);
  printf("Alpha to coeffients\n");
  hexdump((unsigned char*)ALPHA_TO,256);*/
  unsigned int i, j;
  data_t feedback;

  memset(parity,0,NROOTS*sizeof(data_t));

  for(i=0;i<NN-NROOTS-pad;i++){
    feedback = INDEX_OF[data[i] ^ parity[0]];
    if(feedback != A0){      /* feedback term is non-zero */
      for(j=1;j<NROOTS;j++)
	parity[j] ^= ALPHA_TO[MODNN(feedback + GENPOLY[NROOTS-j])];
    }
    /* Shift */
    memmove(&parity[0],&parity[1],sizeof(data_t)*(NROOTS-1));
    if(feedback != A0)
      parity[NROOTS-1] = ALPHA_TO[MODNN(feedback + GENPOLY[0])];
    else
      parity[NROOTS-1] = 0;
  }
}
