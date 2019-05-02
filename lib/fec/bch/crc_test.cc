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
typedef unsigned char uint8_t;
typedef unsigned char crc;
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
crc  crcTable[256];
#define POLYNOMIAL 0x8A  // x^7 + x^6 + x^2 + 1
#define INITIAL_REMAINDER 0x00
#define BCH_INFORMATION_BLOCK 7
void
crcInit(void)
{
    crc  remainder;


    /*
     * Compute the remainder of each possible dividend.
     */
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        /*
         * Start with the dividend followed by zeros.
         */
        remainder = dividend << (WIDTH - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */			
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[dividend] = remainder;
	//std::cout << "Remainder from table : " << int (remainder&0xffff) << std::endl;
    }

}   /* crcInit() */

crc
crcFast(uint8_t const message[], int nBytes, crc const initial_remainder)
{
    uint8_t data;
    crc remainder = initial_remainder;


    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (remainder);

}   /* crcFast() */

void
crcEncoder(std::vector<unsigned char> &message, const crc initial_remainder)
{
    uint8_t data;
    crc remainder = initial_remainder;

    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (int byte = 0; byte < message.size(); ++byte)
    {
        data = message.at(byte) ^ (remainder >> (WIDTH - 8));
        //remainder = crcTable[data] ^ (remainder << 8);
        remainder = crcTable[message.at(byte) ^ remainder];
    }

    //Flip the remainder and move by 1 bit
    remainder ^= 0xFF;
    remainder <<= 1;

    //Set filler bit to 0 (anding with 1111 1110)
    remainder &= 0xFE;

    /*
     * The final remainder is the CRC.
     */
    message.push_back(remainder);
    //return message;
}


void bchEncoder(std::vector<unsigned char> &message)
{
    std::vector<unsigned char> information; // 7 bytes
    std::vector<unsigned char> codewords; // Encoded message

    //Ensure integral information symbols
    while(!(message.size() % BCH_INFORMATION_BLOCK) == 0)
      {
        message.push_back(0x55);
      }

    for(int i = 0; i < message.size(); i += BCH_INFORMATION_BLOCK)
    {
        //Copy 7 information bytes
        std::copy(message.begin() + i, message.begin() + i + BCH_INFORMATION_BLOCK,
                      std::back_inserter(information));
        //BCH encoding
        crcEncoder(information,INITIAL_REMAINDER);

        //Copy encoded information bits
        codewords.insert(codewords.end(), information.begin(), information.end());

        //Clear information bytes
        information.clear();
    }
    message = codewords;
}


int main()
{
  crcInit();
  //hexdump(crcTable,256);
  unsigned char message[] = {0xaa, 0xbb, 0xcd, 0xdd, 0xee, 0xff, 0x11,0x00};
  //unsigned char tail[] = {0xC5,0xC5,0xC5,0xC5,0xC5,0xC5,0xC5,0x79};
  std::vector<unsigned char> info(message, message + sizeof(message)/sizeof(unsigned char));

  bchEncoder(info);
  hexdump(info.data(),info.size());

  //Vector hex dump

  return 0;
}
