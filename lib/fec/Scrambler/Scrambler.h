/* 
 * The Scrambler class
 * -------------------
 * This class implements an additive scrambler as specified by the 
 * CCSDS TM SYNCHRONIZATION AND CHANNEL CODING blue book standard (August 2011).
 * The class is based on the original Reed Solomon codec implemented by Phil Karn
 * Author   : Moses Browne Mwakyanjala
 * Date     : Feb 23rd, 2018
 * Institue : Lulea University of Technology
 * E-mail   : moses.browne.mwakyanjala@ltu.se
 */

#ifndef SCRAMBLER_H
#define SCRAMBLER_H
#include <vector>
#include <algorithm>

#define SCRAMBLER_POLY_LEN 255

typedef unsigned char data_t;
typedef std::vector<unsigned char> BYTEARRAY;

class Scrambler {
 public:
  Scrambler();
  ~Scrambler();
  void Scramble(BYTEARRAY &data);
  void Descramble(BYTEARRAY &data);
  void descramble_soft(float data[],int frame_length);
 private:
  data_t *POLY;

};
#endif

