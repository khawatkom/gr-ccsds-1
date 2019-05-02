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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "genCADU_impl.h"
//#include "fec/rs/rs"


namespace gr {
  namespace ccsds {

    genCADU::sptr
    genCADU::make(int frameLength,std::string sync, int scramble, int rs, int intDepth, std::string len_tag_key)
    {
      return gnuradio::get_initial_sptr
        (new genCADU_impl(frameLength, sync, scramble, rs, intDepth, len_tag_key));
    }

    /*
     * The private constructor
     */
    genCADU_impl::genCADU_impl(int frameLength,std::string sync, int scramble, int rs, int intDepth, std::string len_tag_key)
      : gr::tagged_stream_block("genCADU",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)),len_tag_key),
        d_frameLength(frameLength),d_scramble(scramble == 1),d_rs(rs), d_intDepth(intDepth)
    {
        //Initiliazing RS encoders
        switch(d_rs)
        {
        case 0:
            //No code. Nothing to do
            d_basis = 0;
            d_type  = 0;
            break;
        case 1: //RS(255,223) + Conv
            d_basis = 0;
            d_type  = 0;
            break;
        case 2: //RS(255,223) + Dual basis
            d_basis = 1;
            d_type  = 0;
            break;
        case 3: //RS(255,239) + Conv
            d_basis = 0;
            d_type  = 1;
            break;
        case 4: //RS(255,239) + Dual basis
            d_basis = 1;
            d_type  = 1;
            break;
        default:
            d_basis = 0;
            d_type  = 0;
            //Nothing to do
        }

        //Synchronization pattern
        d_sync = parse_string(sync);
	
        //Reed-Solomon and Scrambler objects
        //RS = new ReedSolomon(16,d_intDepth,d_basis);// False = conventional, True = dual-basis
        //rs =  (new reedSolomon(d_type,d_intDepth,d_basis));
        S  = new Scrambler();

    }

    /*
     * Our virtual destructor.
     */
    genCADU_impl::~genCADU_impl()
    {
      //delete RS;
      delete S;
    }

    int
    genCADU_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = (d_rs) ? d_frameLength + 32*d_intDepth + d_sync.size() : d_frameLength + d_sync.size();
      return noutput_items ;
    }

    unsigned char
    genCADU_impl::parse_hex(char c)
    {
      if ('0' <= c && c <= '9') return c - '0';
      if ('A' <= c && c <= 'F') return c - 'A' + 10;
      if ('a' <= c && c <= 'f') return c - 'a' + 10;
      std::abort();
    }

    std::vector<unsigned char>
    genCADU_impl::parse_string(const std::string & s)
    {
      if (s.size() % 2 != 0) std::abort();
      std::vector<unsigned char> result(s.size() / 2);

      for (std::size_t i = 0; i != s.size() / 2; ++i)
        result[i] = 16 * parse_hex(s[2 * i]) + parse_hex(s[2 * i + 1]);

      return result;
    }

    int
    genCADU_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      int total_len;
      //ezpwd::RS_CCSDS<255,223> rs;

      reedSolomon rs(d_type,d_intDepth,d_basis);
      //Copy pdu from circular buffer to local buffer
      buffer.insert(buffer.end(), in, in +  d_frameLength);

      //Optional scrambling and Reed-Solomon. TO DO: Turbo and LDPC
      //if (d_rs) RS->Encode_RS(buffer);
      if (d_rs > 0) rs.encode(buffer);
      if (d_scramble) S->Scramble(buffer);

      //Insert sync word
      buffer.insert(buffer.begin(), d_sync.begin(), d_sync.end());

      //Copy from local buffer to circular buffer
      std::copy(buffer.begin(),buffer.end(),out);

      //Clear the local buffer
      total_len = buffer.size();
      buffer.clear();

      // Tell runtime system how many output items we produced.
      return total_len;
    }

  } /* namespace ccsds */
} /* namespace gr */

