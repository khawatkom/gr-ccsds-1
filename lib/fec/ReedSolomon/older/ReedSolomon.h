/* 
 * The Reed Solomon codec class
 * ----------------------------
 * This class implements Reed Solomon coder and decoder as specified by the 
 * CCSDS TM SYNCHRONIZATION AND CHANNEL CODING blue book standard (August 2011).
 * The class is based on the original Reed Solomon codec implemented by Phil Karn
 * Author   : Moses Browne Mwakyanjala
 * Date     : Feb 18th, 2018
 * Institue : Lulea University of Technology
 * E-mail   : moses.browne.mwakyanjala@ltu.se
 */

#ifndef REEDSOLOMON_H
#define REEDSOLOMON_H
#include <vector>
#include "ReedSolomon_CCSDS.h"

/* Class variables
 * ---------------
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
 * program.*/

class ReedSolomon {
 public:
  /* Class constructor
     -----------------
     @param E : Number of errors.
              - Valid values
                [1] E = 16 corresponding to RS(255,223)
		[2] E = 8  corresponding to RS(255,239)
     @param I : Interleave depth
              - Valid values
                ~~ 1, 2, 3, 4, 5 and 8
     @param D : Dual-basis representation
              - Valid values
                [1] true : Dual-basis representation
                [2] false: Conventional representation
     --------------------------------------------------
   */
  ReedSolomon(int E, int I,bool D);

  /* Class destructor
     ----------------
     Frees memory located for ALPHA_TO, INDEX_OF and GENPOLY arrays
     --------------------------------------------------------------
  */
  ~ReedSolomon();

  /* Encode_RS function
     ------------------
     @param data : A std::vector<unsigned char> with raw data
     @param dual : A bool object which is set to true if 
                   dual-basis representation is used
     --------------------------------------------------------
   */
  void Encode_RS(std::vector<unsigned char> &data);

  /* Decode_RS function
     ------------------
     @param data : A std::vector<unsigned char> with RS data
     @param num_errors : Number of errors corrected 
     @param dual : A bool object which is set to true if 
                   dual-basis representation is used
     -------------------------------------------------------
  */
  void Decode_RS(std::vector<unsigned char> &data, int &num_errors);


 protected:
  void encode_rs_8(data_t *data, data_t *parity,int pad);
  int  decode_rs_8(data_t *data, int *eras_pos, int no_eras, int pad);
  int  MODNN(int x);
  int  MINNN(int a, int b);
 private:
  bool dual;
  int MM;
  int NN;
  int NROOTS;
  int FCR;
  int PRIM;
  int IPRIM;
  data_t A0;
  data_t* ALPHA_TO;
  data_t* INDEX_OF;
  data_t* GENPOLY;
};
#endif

