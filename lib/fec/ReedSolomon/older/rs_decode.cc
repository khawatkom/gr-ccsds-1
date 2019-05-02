/*
 * @author Phil-Karn
 * Copyright Feb 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
  //#include <ax100/ax100.h>
  //#include <ax100/ax100.h>
#include <string.h>
#include "rs_decode.h"

  int decode_rs_8(data_t *data, int *eras_pos, int no_eras, int pad){
    int retval;

    if(pad < 0 || pad > 222) {
      return -1;
    }

    /* The guts of the Reed-Solomon decoder, meant to be #included
     * into a function body with the following typedefs, macros and variables supplied
     * according to the code parameters:
     * data_t - a typedef for the data symbol
     * data_t data[] - array of NN data and parity symbols to be corrected in place
     * retval - an integer lvalue into which the decoder's return code is written
     * NROOTS - the number of roots in the RS code generator polynomial,
     *          which is the same as the number of parity symbols in a block.
     Integer variable or literal.
     * NN - the total number of symbols in a RS block. Integer variable or literal.
     * PAD - the number of pad symbols in a block. Integer variable or literal.
     * ALPHA_TO - The address of an array of NN elements to convert Galois field
     *            elements in index (log) form to polynomial form. Read only.
     * INDEX_OF - The address of an array of NN elements to convert Galois field
     *            elements in polynomial form to index (log) form. Read only.
     * MODNN - a function to reduce its argument modulo NN. May be inline or a macro.
     * FCR - An integer literal or variable specifying the first consecutive root of the
     *       Reed-Solomon generator polynomial. Integer variable or literal.
     * PRIM - The primitive root of the generator poly. Integer variable or literal.
     * DEBUG - If set to 1 or more, do various internal consistency checking. Leave this
     *         undefined for production code
     * The memset(), memmove(), and memcpy() functions are used. The appropriate header
     * file declaring these functions (usually <string.h>) must be included by the calling
     * program.
     */

    int deg_lambda, el, deg_omega;
    int i, j, r,k;
    data_t u,q,tmp,num1,num2,den,discr_r;
    data_t lambda[NROOTS+1], s[NROOTS];	/* Err+Eras Locator poly
					 * and syndrome poly */
    data_t b[NROOTS+1], t[NROOTS+1], omega[NROOTS+1];
    data_t root[NROOTS], reg[NROOTS+1], loc[NROOTS];
    int syn_error, count;

    /* form the syndromes; i.e., evaluate data(x) at roots of g(x) */
    for(i=0;i<NROOTS;i++)
      s[i] = data[0];

    for(j=1;j<NN-PAD;j++){
      for(i=0;i<NROOTS;i++){
	if(s[i] == 0){
	  s[i] = data[j];
	} else {
	  s[i] = data[j] ^ ALPHA_TO[MODNN(INDEX_OF[s[i]] + (FCR+i)*PRIM)];
	}
      }
    }

    /* Convert syndromes to index form, checking for nonzero condition */
    syn_error = 0;
    for(i=0;i<NROOTS;i++){
      syn_error |= s[i];
      s[i] = INDEX_OF[s[i]];
    }

    if (!syn_error) {
      /* if syndrome is zero, data[] is a codeword and there are no
       * errors to correct. So return data[] unmodified
       */
      count = 0;
      goto finish;
    }
    memset(&lambda[1],0,NROOTS*sizeof(lambda[0]));
    lambda[0] = 1;

    if (no_eras > 0) {
      /* Init lambda to be the erasure locator polynomial */
      lambda[1] = ALPHA_TO[MODNN(PRIM*(NN-1-eras_pos[0]))];
      for (i = 1; i < no_eras; i++) {
	u = MODNN(PRIM*(NN-1-eras_pos[i]));
	for (j = i+1; j > 0; j--) {
	  tmp = INDEX_OF[lambda[j - 1]];
	  if(tmp != A0)
	    lambda[j] ^= ALPHA_TO[MODNN(u + tmp)];
	}
      }


    finish:
      if(eras_pos != NULL){
	for(i=0;i<count;i++)
	  eras_pos[i] = loc[i];
      }
      retval = count;

      return retval;
    }


#ifdef __cplusplus
  }
#endif
