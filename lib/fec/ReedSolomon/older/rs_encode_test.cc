/* Reed-Solomon encoder
 * Copyright 2002, Phil Karn, KA9Q
 * May be used under the terms of the GNU General Public License (GPL)
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include "ReedSolomon.h"
//#include "rs_encode.h"
//#include "rs_decode.h"
#include <iostream>
#include "debug.h"
int main()
{
  //Generate bogus data
  std::vector<unsigned char> pkt;
  std::vector<unsigned char> pkt2;
  unsigned char tal[] = { 0x8d, 0xef, 0xec, 0x86, 0xfa, 0x99, 0xaf, 0x7b };
  for (int i = 0; i < 128; i++)
    {
      pkt.push_back(tal[i%8]);
      pkt2.push_back(tal[i%8]);
    }
  //std::cout <<"Original data. Frame size = " << pkt.size() << std::endl; 
  //hexdump(pkt.data(),pkt.size());
  
  unsigned char *data;
  data = (unsigned char *)malloc(pkt.size());
  unsigned char *parity;
  parity = (unsigned char *)malloc(32);

  //Removing AX25 header and copy pkt to data. New packet size pkt.size() - 16
  std::copy(pkt.begin(),pkt.end(),data);

  //Reed-Solomon coding
  //encode_rs_8(data,parity,255 - (pkt.size()) - 32);
  //std::cout << "parity data" << std::endl;

      
  //Appending parity bits
  //for (int i = 0; i < 32; i++)
    //{
      //pkt.push_back(parity[i]);
    //}
 
  //RS.Encode_RS(data);
  // std::cout <<"Encoded data C. Frame size = " << pkt.size() << std::endl; 
  //hexdump(pkt.data(),pkt.size());


  
  std::cout <<"  -------Testing C++ implementation-------- " << std::endl;
  std::cout <<"  ========================================= " << std::endl;
  std::cout <<  std::endl <<  std::endl ;

  //Initializing RS object
  bool dual = true;
  ReedSolomon RS(16,2,dual);
  RS.Encode_RS(pkt2);
  std::cout <<"RS Encoded packet (C++)" << std::endl;
  std::cout <<"Frame size = " << pkt2.size() << std::endl;
  std::cout << std::endl << std::endl;
  hexdump(pkt2.data(),pkt2.size());
  
  //Introducing corruption to the packet
  pkt2[0] = 0xdd;
  pkt2[1] = 0xdd;
  pkt2[2] = 0xdd;
  pkt2[3] = 0xdd;
  pkt2[4] = 0xdd;
  pkt2[5] = 0xdd;
  pkt2[6] = 0xdd;
  pkt2[7] = 0xdd;
  pkt2[8] = 0xdd;
  pkt2[9] = 0xdd;
  pkt2[10] = 0xdd;
  pkt2[11] = 0xdd;
  pkt2[12] = 0xdd;
  pkt2[13] = 0xdd;
  pkt2[14] = 0xdd;
  //pkt2[15] = 0xdd;
  //pkt2[16] = 0xdd;
  //pkt2.erase(pkt.begin() + 10);
  std::cout <<"Corrupted packet" << std::endl;
  std::cout <<"Frame size = " << pkt2.size() << std::endl;
  hexdump(pkt2.data(),pkt2.size());

  //RS decoding
   int rs_res;
   RS.Decode_RS(pkt2,rs_res);
   // decode_rs_8(pkt2.data(), NULL, 0, 255 - pkt2.size());
  std::cout << std::endl << std::endl;

  std::cout << " Reed Solomon result = " << rs_res << std::endl;
  hexdump(pkt2.data(),pkt2.size());
  return 0;
}
