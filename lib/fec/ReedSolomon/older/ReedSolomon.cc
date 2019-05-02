/* 
 * The Reed Solomon codec class implementation
 * -------------------------------------------
 * This class implements Reed Solomon coder and decoder as specified by the 
 * CCSDS TM SYNCHRONIZATION AND CHANNEL CODING blue book standard (August 2011).
 * The class is based on the original Reed Solomon codec implemented by Phil Karn
 * Author   : Moses Browne Mwakyanjala
 * Date     : Feb 18th, 2018
 * Institue : Lulea University of Technology
 * E-mail   : moses.browne.mwakyanjala@ltu.se
 */
#include <string.h>
#include <iostream>
#include "ReedSolomon.h"
#include <stdlib.h>
#include "debug.h"

ReedSolomon::ReedSolomon(int E, int I,bool D)
{
  dual = D;
  //Initializing CCSDS RS parameters
  switch (E)
    {
    case 16:
      //Alpha index
      ALPHA_TO = new data_t[256];
      memcpy(ALPHA_TO, E16_alphato,256);

      //Indicies
      INDEX_OF = new data_t[256];
      memcpy(INDEX_OF,E16_indexof,256);
  
      //Generator polynomial
      GENPOLY = new data_t[33];
      memcpy(GENPOLY,E16_genpoly,33);
      
      //Variables
      MM     = 8;
      NN     = 255;
      NROOTS = 32;
      A0     = 255;
      FCR    = 112;
      PRIM   = 11;
      IPRIM  = 116;
      break;
      
    case 8:
      //MM = 8;
      //NN = 255;
      //NROOTS = 16;
      //A0 = 255;
      //ALPHA_TO = CCSDS_E16_alpha_to;
      //INDEX_OF = CCSDS_E16_index_of;
      //GENPOLY  = CCSDS_E16_poly;
      break;
    default:
      std::cerr<<"Incorrect value of E" << std::endl;
    }
}

ReedSolomon::~ReedSolomon()
{
  delete[] ALPHA_TO;
  delete[] INDEX_OF;
  delete[] GENPOLY;
}

void ReedSolomon::Encode_RS(std::vector<unsigned char> &data)
{
  //Reed solomon data and parity strings
  int len = data.size();
  data_t rs_data[len];
  data_t parity[NROOTS];
  if(!dual){
    std::copy(data.begin(),data.end(),rs_data);
    encode_rs_8(rs_data,parity,NN - data.size()- NROOTS);
  }
  else{
    //Dual-basis to conventional transformation
    for(int i = 0; i < len; i++)
      rs_data[i] = Tal1tab[data[i]];
    encode_rs_8(rs_data,parity,NN - data.size()- NROOTS);
    //Conventional to dual-basis transformation
    for(int j =0; j<NROOTS; j++)
      parity[j] = Taltab[parity[j]];
  }

  //Appending parity bytes
  data.insert(data.end(), parity, parity +  NROOTS);
}

void ReedSolomon::Decode_RS(std::vector<unsigned char> &rsdata, int &num_errors)
{
  //Reed solomon data and parity strings
  int len = rsdata.size();
  data_t d_rsdata[len];
  //d_rsdata = (data_t *)malloc(sizeof(data_t)*len);
  //Copying from std::vector<> to unsigned char*
  if(!dual){
    std::copy(rsdata.begin(),rsdata.end(),d_rsdata); 
    num_errors = decode_rs_8(d_rsdata, NULL, 0, NN - len);
  }else{
    //Dual-basis to conventional transformation
    for(int i = 0; i < len; i++)
      d_rsdata[i] = Tal1tab[rsdata[i]];
    num_errors = decode_rs_8(d_rsdata, NULL, 0, NN - len);
    //Conventional to dual-basis transformation
    for(int j =0; j<len; j++)
      d_rsdata[j] = Taltab[d_rsdata[j]];
  }

  //clear rsdata and insert decoded data
  rsdata.clear();
  //Copying from  unsigned char* to std::vector<>
  rsdata.insert(rsdata.end(), d_rsdata, d_rsdata + len - NROOTS);
  return ;
}

int ReedSolomon::MODNN(int x){
  while (x >= 255) {
    x -= 255;
    x = (x >> 8) + (x & 255);
  }
  return x;
}

int ReedSolomon::MINNN(int a, int b)
{
  return  ((a) < (b) ? (a) : (b));
}

void ReedSolomon::encode_rs_8(data_t *data, data_t *parity,int pad){
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

int ReedSolomon::decode_rs_8(data_t *data, int *eras_pos, int no_eras, int pad) {
  int retval;
  int PAD = pad;

  if(pad < 0 || pad > 222) {
    return -1;
  }
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

#if DEBUG >= 1
    /* Test code that verifies the erasure locator polynomial just constructed
       Needed only for decoder debugging. */

    /* find roots of the erasure location polynomial */
    for(i=1;i<=no_eras;i++)
      reg[i] = INDEX_OF[lambda[i]];

    count = 0;
    for (i = 1,k=IPRIM-1; i <= NN; i++,k = MODNN(k+IPRIM)) {
      q = 1;
      for (j = 1; j <= no_eras; j++)
	if (reg[j] != A0) {
	  reg[j] = MODNN(reg[j] + j);
	  q ^= ALPHA_TO[reg[j]];
	}
      if (q != 0)
	continue;
      /* store root and error location number indices */
      root[count] = i;
      loc[count] = k;
      count++;
    }
    if (count != no_eras) {
      printf("count = %d no_eras = %d\n lambda(x) is WRONG\n",count,no_eras);
      count = -1;
      goto finish;
    }
#if DEBUG >= 2
    printf("\n Erasure positions as determined by roots of Eras Loc Poly:\n");
    for (i = 0; i < count; i++)
      printf("%d ", loc[i]);
    printf("\n");
#endif
#endif
  }
  for(i=0;i<NROOTS+1;i++)
    b[i] = INDEX_OF[lambda[i]];

  /*
   * Begin Berlekamp-Massey algorithm to determine error+erasure
   * locator polynomial
   */
  r = no_eras;
  el = no_eras;
  while (++r <= NROOTS) {	/* r is the step number */
    /* Compute discrepancy at the r-th step in poly-form */
    discr_r = 0;
    for (i = 0; i < r; i++){
      if ((lambda[i] != 0) && (s[r-i-1] != A0)) {
	discr_r ^= ALPHA_TO[MODNN(INDEX_OF[lambda[i]] + s[r-i-1])];
      }
    }
    discr_r = INDEX_OF[discr_r];	/* Index form */
    if (discr_r == A0) {
      /* 2 lines below: B(x) <-- x*B(x) */
      memmove(&b[1],b,NROOTS*sizeof(b[0]));
      b[0] = A0;
    } else {
      /* 7 lines below: T(x) <-- lambda(x) - discr_r*x*b(x) */
      t[0] = lambda[0];
      for (i = 0 ; i < NROOTS; i++) {
	if(b[i] != A0)
	  t[i+1] = lambda[i+1] ^ ALPHA_TO[MODNN(discr_r + b[i])];
	else
	  t[i+1] = lambda[i+1];
      }
      if (2 * el <= r + no_eras - 1) {
	el = r + no_eras - el;
	/*
	 * 2 lines below: B(x) <-- inv(discr_r) *
	 * lambda(x)
	 */
	for (i = 0; i <= NROOTS; i++)
	  b[i] = (lambda[i] == 0) ? A0 : MODNN(INDEX_OF[lambda[i]] - discr_r + NN);
      } else {
	/* 2 lines below: B(x) <-- x*B(x) */
	memmove(&b[1],b,NROOTS*sizeof(b[0]));
	b[0] = A0;
      }
      memcpy(lambda,t,(NROOTS+1)*sizeof(t[0]));
    }
  }

  /* Convert lambda to index form and compute deg(lambda(x)) */
  deg_lambda = 0;
  for(i=0;i<NROOTS+1;i++){
    lambda[i] = INDEX_OF[lambda[i]];
    if(lambda[i] != A0)
      deg_lambda = i;
  }
  /* Find roots of the error+erasure locator polynomial by Chien search */
  memcpy(&reg[1],&lambda[1],NROOTS*sizeof(reg[0]));
  count = 0;		/* Number of roots of lambda(x) */
  for (i = 1,k=IPRIM-1; i <= NN; i++,k = MODNN(k+IPRIM)) {
    q = 1; /* lambda[0] is always 0 */
    for (j = deg_lambda; j > 0; j--){
      if (reg[j] != A0) {
	reg[j] = MODNN(reg[j] + j);
	q ^= ALPHA_TO[reg[j]];
      }
    }
    if (q != 0)
      continue; /* Not a root */
    /* store root (index-form) and error location number */
#if DEBUG>=2
    printf("count %d root %d loc %d\n",count,i,k);
#endif
    root[count] = i;
    loc[count] = k;
    /* If we've already found max possible roots,
     * abort the search to save time
     */
    if(++count == deg_lambda)
      break;
  }
  if (deg_lambda != count) {
    /*
     * deg(lambda) unequal to number of roots => uncorrectable
     * error detected
     */
    count = -1;
    goto finish;
  }
  /*
   * Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo
   * x**NROOTS). in index form. Also find deg(omega).
   */
  deg_omega = deg_lambda-1;
  for (i = 0; i <= deg_omega;i++){
    tmp = 0;
    for(j=i;j >= 0; j--){
      if ((s[i - j] != A0) && (lambda[j] != A0))
	tmp ^= ALPHA_TO[MODNN(s[i - j] + lambda[j])];
    }
    omega[i] = INDEX_OF[tmp];
  }

  /*
   * Compute error values in poly-form. num1 = omega(inv(X(l))), num2 =
   * inv(X(l))**(FCR-1) and den = lambda_pr(inv(X(l))) all in poly-form
   */
  for (j = count-1; j >=0; j--) {
    num1 = 0;
    for (i = deg_omega; i >= 0; i--) {
      if (omega[i] != A0)
	num1  ^= ALPHA_TO[MODNN(omega[i] + i * root[j])];
    }
    num2 = ALPHA_TO[MODNN(root[j] * (FCR - 1) + NN)];
    den = 0;

    /* lambda[i+1] for i even is the formal derivative lambda_pr of lambda[i] */
    for (i = MINNN(deg_lambda,NROOTS-1) & ~1; i >= 0; i -=2) {
      if(lambda[i+1] != A0)
	den ^= ALPHA_TO[MODNN(lambda[i+1] + i * root[j])];
    }
#if DEBUG >= 1
    if (den == 0) {
      printf("\n ERROR: denominator = 0\n");
      count = -1;
      goto finish;
    }
#endif
    /* Apply error to data */
    if (num1 != 0 && loc[j] >= PAD) {
      data[loc[j]-PAD] ^= ALPHA_TO[MODNN(INDEX_OF[num1] + INDEX_OF[num2] + NN - INDEX_OF[den])];
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
