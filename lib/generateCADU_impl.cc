/* -*- c++ -*- */
/* 
 * Copyright 2018
 * Author      : Moses Browne Mwakyanjala
 * Institution : Lulea University of Techology, Sweden
 * E-mail      : mbkitine@gmail.com ; moses.browne.mwakyanjala@ltu.se
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
#include "generateCADU_impl.h"
#include "fec/ReedSolomon/ReedSolomon.h"
#include "fec/Scrambler/Scrambler.h"

namespace gr {
  namespace ccsds {

    generateCADU::sptr
    generateCADU::make(int frameLength,std::string sync, int scramble, int rs, int intDepth)
    {
      return gnuradio::get_initial_sptr
        (new generateCADU_impl(frameLength, sync, scramble, rs, intDepth));
    }

    /*
     * The private constructor
     */
    generateCADU_impl::generateCADU_impl(int frameLength,std::string sync, int scramble, int rs, int intDepth)
      : gr::sync_block("generateCADU",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
		       gr::io_signature::make(0, 0, 0)),
	d_frameLength(frameLength),d_scramble(scramble == 1),d_rs(rs >= 1), d_basis(rs >= 2), d_intDepth(intDepth)
	
    {
      set_output_multiple(d_frameLength);
      //Registering output port
      message_port_register_out(pmt::mp("out"));
      d_sync = parse_string(sync);
    }

    /*
     * Our virtual destructor.
     */
    generateCADU_impl::~generateCADU_impl()
    {
    }

    unsigned char
    generateCADU_impl::parse_hex(char c)
    {
      if ('0' <= c && c <= '9') return c - '0';
      if ('A' <= c && c <= 'F') return c - 'A' + 10;
      if ('a' <= c && c <= 'f') return c - 'a' + 10;
      std::abort();
    }
    
    std::vector<unsigned char>
    generateCADU_impl::parse_string(const std::string & s)
    {
      if (s.size() % 2 != 0) std::abort();
      std::vector<unsigned char> result(s.size() / 2);

      for (std::size_t i = 0; i != s.size() / 2; ++i)
        result[i] = 16 * parse_hex(s[2 * i]) + parse_hex(s[2 * i + 1]);

      return result;
    }
    int
    generateCADU_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];

      //Reed-Solomon and Scrambler objects
      ReedSolomon RS(16,d_intDepth,d_basis);// False = conventional, True = dual-basis
      Scrambler S;

      //Buffers
      unsigned char *frameBuffer1 = (unsigned char*)malloc(d_frameLength*sizeof(unsigned char));
      std::vector<unsigned char> frameBuffer2;

      //The work function engine
      for(int i = 0; (i + d_frameLength) < noutput_items; i += d_frameLength)
	{
	  //Copying data from input stream
	  memcpy(frameBuffer1,in + i + d_frameLength,d_frameLength);

	  //Copying frame into std::vector buffer
	  frameBuffer2.insert(frameBuffer2.begin(),frameBuffer1, frameBuffer1 + d_frameLength);

          //Optional scrambling and Reed-Solomon. TO DO: Turbo and LDPC
	  if (d_rs) RS.Encode_RS(frameBuffer2);
          if (d_scramble) S.Scramble(frameBuffer2);
	   
	  //Insert sync word
	  frameBuffer2.insert(frameBuffer2.begin(), d_sync.begin(), d_sync.end());
	  
	  //Transmitting PDU
	  pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(frameBuffer2.data(),frameBuffer2.size())));
	  message_port_pub(pmt::mp("out"), pdu);

	  //Clear buffer
	  frameBuffer2.clear();
	}

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ccsds */
} /* namespace gr */

