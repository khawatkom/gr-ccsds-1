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

#ifndef INCLUDED_CCSDS_ENCODERS_IMPL_H
#define INCLUDED_CCSDS_ENCODERS_IMPL_H

#include <ccsds/encodeRS.h>
#include "fec/rs/reedsolomon.h"
namespace gr {
  namespace ccsds {

    class encodeRS_impl : public encodeRS
    {
    private:
        int d_basis;
        int d_type;
        int d_intDepth;
        int d_parity;
        int d_cbSize;
        std::vector<unsigned char> buffer;
        uint64_t d_tag_position;
        std::string d_out_tag_key;

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      encodeRS_impl(int code, int cbSize, int intDepth, std::string out_tag_key, std::string len_tag_key);
      ~encodeRS_impl();

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_ENCODERS_IMPL_H */

