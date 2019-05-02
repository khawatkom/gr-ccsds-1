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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "encodeRS_impl.h"

namespace gr {
  namespace ccsds {

    encodeRS::sptr
    encodeRS::make(int code, int cbSize,int intDepth, std::string out_tag_key, std::string len_tag_key)
    {
      return gnuradio::get_initial_sptr
        (new encodeRS_impl(code, cbSize, intDepth, out_tag_key, len_tag_key));
    }

    /*
     * The private constructor
     */
    encodeRS_impl::encodeRS_impl(int code, int cbSize, int intDepth, std::string out_tag_key, std::string len_tag_key)
      : gr::tagged_stream_block("encodeRS",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)), len_tag_key),
       d_intDepth(intDepth), d_cbSize(cbSize),d_tag_position(0), d_out_tag_key(out_tag_key)
    {
        switch (code) {
        case 1: //RS(255,223) + Conv
            d_basis = 0;
            d_type  = 0;
            d_parity= 32;
            break;
        case 2: //RS(255,223) + Dual basis
            d_basis = 1;
            d_type  = 0;
            d_parity= 32;
            break;
        case 3: //RS(255,239) + Conv
            d_basis = 0;
            d_type  = 1;
            d_parity= 16;
            break;
        case 4: //RS(255,239) + Dual basis
            d_basis = 1;
            d_type  = 1;
            d_parity= 16;
            break;
        default:
            d_basis = 0;
            d_type  = 0;
            d_parity= 32;
        }
    }

    /*
     * Our virtual destructor.
     */
    encodeRS_impl::~encodeRS_impl()
    {
    }

    int
    encodeRS_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = d_cbSize + d_parity*d_intDepth;
      return noutput_items ;
    }

    int
    encodeRS_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
        int total_len;

        reedSolomon rs(d_type,d_intDepth,d_basis);

        //Copy pdu from circular buffer to local buffer
        buffer.insert(buffer.end(), in, in +  d_cbSize);

        //Reed-Solomon encoding
        rs.encode(buffer);

        //Copy from local buffer to circular buffer
        std::copy(buffer.begin(),buffer.end(),out);

        //Clear the local buffer
        total_len = buffer.size();

        //Add output tag
        add_item_tag(0, d_tag_position,pmt::string_to_symbol(d_out_tag_key), pmt::from_long(total_len));
        d_tag_position += total_len;
        buffer.clear();

        // Tell runtime system how many output items we produced.
        return total_len;
    }

  } /* namespace ccsds */
} /* namespace gr */

