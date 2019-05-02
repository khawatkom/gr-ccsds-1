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
typedef unsigned short crc;
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
crc  crcTable[256];
#define POLYNOMIAL 0x1021  /* 11011 followed by 0's */
crc
crcNaive(crc const message)
{
    crc  remainder;	


    /*
     * Initially, the dividend is the remainder.
     */
    remainder = message;
    /*
     * For each bit position in the message....
     */
    for (crc bit = 8; bit > 0; --bit)
    {
        /*
         * If the uppermost bit is a 1...
         */
        if (remainder & 0x80)
        {
            /*
             * XOR the previous remainder with the divisor.
             */
            remainder ^= POLYNOMIAL;
        }

        /*
         * Shift the next bit of the message into the remainder.
         */
        remainder = (remainder << 1);
    }

    /*
     * Return only the relevant bits of the remainder as CRC.
     */
    return (remainder >> 4);

}   /* crcNaive() */

crc
crcSlow(unsigned char const message[], int nBytes, crc const initial_remainder)
{
    crc  remainder = initial_remainder;	


    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (int byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        //unsigned char BYTE = message[byte] << (WIDTH -8);
        remainder ^= (message[byte] << (WIDTH - 8));
	//std::cout << "CRC Slow: Remainder : " << int (BYTE & 0xffff) << std::endl;
        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (unsigned char bit = 8; bit > 0; --bit)
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
    }

    /*
     * The final remainder is the CRC result.
     */
    return (remainder);

}   /* crcSlow() */

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

int main()
{
  /*
  {
  //Naive CRC
  crc message = 0xE5;
  crc rem = crcNaive(message);
  rem &=0xff;
  std::cout << "Naive CRC: " << int(rem) << std::endl;
  }
  std::cout << "WIDTH : " <<  WIDTH << std::endl;
  std::cout << "TOPBIT: " <<  TOPBIT<< std::endl;
  unsigned char message[] = {0xaa, 0xbb, 0xcd, 0xdd, 0xee, 0xff, 0x11, 0x22};
  crc rem = crcSlow(message,8,0xffff);
  rem &= 0xffff;
  std::cout << "Slow CRC: " << int(rem) << std::endl;
  std::cout << "0xffff^0xaa00 = " << int((0xffff^0xaa00)&0xffff) << std::endl;
  unsigned char aa = 0xaa;
  std::cout << "0xaa << 8 = " << int((aa << 8)&0xffff) << std::endl;*/
  crcInit();
  //hexdump(crcTable,256);
  unsigned char message[] = {0xaa, 0xbb, 0xcd, 0xdd, 0xee, 0xff, 0x11, 0x22};

  /*
  unsigned char tal[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22 };
  int message_size = 1784;
  unsigned char *message = (unsigned char*)malloc(message_size);
  for (int i = 0; i < message_size; i++)
    {
      message[i] = tal[i%8];
      }*/
  
  crc rem = crcSlow(message,8,0xffff);
  rem &= 0xffff;
  std::cout << "Fast CRC: " << int(rem) << std::endl;
  unsigned char aa = 0xaa;
  crc bb  = aa << 8;
  std::cout << "0xaa << 8 = " << int(bb&0xffff) << std::endl;
  return 0;
}
