/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_CCSDS_RECOVERCADU_IMPL_H
#define INCLUDED_CCSDS_RECOVERCADU_IMPL_H

#include <ccsds/recoverCADU.h>
#include "fec/Scrambler/Scrambler.h"
#include <gnuradio/blocks/pack_k_bits.h>
namespace gr {
  namespace ccsds {

    class recoverCADU_impl : public recoverCADU
    {
    private:
        int d_frame_length;
        bool d_scramble;
        std::vector<tag_t> d_tags;
        std::vector<tag_t>::iterator d_tags_itr;
        blocks::kernel::pack_k_bits *d_pack;
        pmt::pmt_t key;
        Scrambler *S;
        std::vector<unsigned char> frameBuffer;
        int d_ambiguity;
        unsigned char swap(unsigned char oct);
	unsigned char swapIQ(unsigned char in);
	unsigned char IQSWAP[256];


     public:
      recoverCADU_impl(int frame_length, int scramble, std::string tag_name);
      ~recoverCADU_impl();
      void ambResolution(int ambiguity, unsigned char frame_data[],int frame_length);
      //uint8_t swapIQ(uint8_t in);
      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_RECOVERCADU_IMPL_H */

