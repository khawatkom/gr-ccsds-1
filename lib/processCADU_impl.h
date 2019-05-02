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

#ifndef INCLUDED_CCSDS_PROCESSCADU_IMPL_H
#define INCLUDED_CCSDS_PROCESSCADU_IMPL_H

#include <ccsds/processCADU.h>
#include <gnuradio/blocks/pack_k_bits.h>
//#include "fec/ReedSolomon/ReedSolomon.h"
#include "fec/rs/reedsolomon.h"
#include "fec/Scrambler/Scrambler.h"

namespace gr {
  namespace ccsds {

    class processCADU_impl : public processCADU
    {
     private:
      int d_frameLength;
      bool d_scramble;
      int d_rs;
      int d_basis;
      int d_type;
      int d_intDepth;
      std::vector<tag_t> d_tags;
      std::vector<tag_t>::iterator d_tags_itr;
      blocks::kernel::pack_k_bits *d_pack;
      pmt::pmt_t key;
      //ReedSolomon *RS;
      Scrambler *S;
      std::vector<unsigned char> frameBuffer;
      std::vector<int> errors;
     public:
      processCADU_impl(int frameLength, int scramble, int rs, int intDepth, std::string tagName);
      ~processCADU_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_PROCESSCADU_IMPL_H */

