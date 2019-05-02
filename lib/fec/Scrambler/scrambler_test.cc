/* Reed-Solomon encoder
 * Copyright 2002, Phil Karn, KA9Q
 * May be used under the terms of the GNU General Public License (GPL)
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <iostream>
#include "debug.h"
#include "Scrambler.h"
using namespace std;
int main()
{
  //Generate bogus data
  std::vector<unsigned char> pkt;
  std::vector<unsigned char> pkt2;
  unsigned char tal[] = { 0x8d, 0xef, 0xec, 0x86, 0xfa, 0x99, 0xaf, 0x7b };
  for (int i = 0; i < 700; i++)
    {
      pkt.push_back(tal[i%8]);
      pkt2.push_back(tal[i%8]);
    }

  //Print packet before scrambling
  cout << "Packet before scrambling" << endl;
  hexdump(pkt.data(),pkt.size());

  //Scrambled packet
  Scrambler S;
  S.Scramble(pkt);
  cout << "Packet after scrambling" << endl;
  hexdump(pkt.data(),pkt.size());

  //Descrambled packt
  S.Descramble(pkt);
  cout << "Packet after descrambling" << endl;
  hexdump(pkt.data(),pkt.size());  
  return 0;
}
