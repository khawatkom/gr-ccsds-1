/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_CCSDS_GENERATECLTU_IMPL_H
#define INCLUDED_CCSDS_GENERATECLTU_IMPL_H

#include <ccsds/generateCLTU.h>
#include "fec/Scrambler/Scrambler.h"
#define BCH_INFORMATION_BLOCK 7
typedef unsigned char uint8_t;
typedef unsigned char crc;
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0x8A
#define INITIAL_REMAINDER 0x00
namespace gr {
  namespace ccsds {

    class generateCLTU_impl : public generateCLTU
    {
     private:
      unsigned char *crcTable;
      void crcInit();
      void cltu(pmt::pmt_t pmt_msg);
      bool d_scramble;
      Scrambler S;
      void bchEncoder(std::vector<unsigned char> &message);
      void crcEncoder(std::vector<unsigned char> &message, const crc initial_remainder);
     public:
      generateCLTU_impl(int scramble);
      ~generateCLTU_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_GENERATECLTU_IMPL_H */

