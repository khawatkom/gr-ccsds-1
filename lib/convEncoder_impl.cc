/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
#include "convEncoder_impl.h"

namespace gr {
  namespace ccsds {

    convEncoder::sptr
    convEncoder::make(int invert)
    {
      return gnuradio::get_initial_sptr
	(new convEncoder_impl(invert));
    }

    /*
     * The private constructor
     */
    convEncoder_impl::convEncoder_impl(int invert)
      : gr::sync_interpolator("convEncoder",
			      gr::io_signature::make(1, 1, sizeof(unsigned char)),
			      gr::io_signature::make(1, 1, sizeof(unsigned char)), 2)
    {
      d_invert = (invert==1) ? true : false;
      for(int unsigned i = 0; i < 7; i++) state.push_back(0x00);
    }

    /*
     * Our virtual destructor.
     */
    convEncoder_impl::~convEncoder_impl()
    {
    }

    int
    convEncoder_impl::work(int noutput_items,
			   gr_vector_const_void_star &input_items,
			   gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      std::vector<unsigned char> bits(2);
      for(int i = 0; i < noutput_items/2; i++)
	{
	  //Removing the first element
	  state.erase(state.begin());
	  //Insert new element
	  state.push_back(in[i]);
	  //Generate output bits
	  bits[0] = state.at(0) ^ state.at(3) ^ state.at(4) ^ state.at(5) ^ state.at(6); //G1 = 1111001 (Taken in reverse)
	  bits[1] = state.at(0) ^ state.at(1) ^ state.at(3) ^ state.at(4) ^ state.at(6); //G2 = 1011011 (Taken in reverse)
	  //Flip the second bit
	  bits[1] = d_invert ? ~bits[1]&0x01 : bits[1];
	  //Copying output bits
	  memcpy(out,bits.data(),2);
	  bits.clear();
	  out +=2;
	  //std::cout <<" Register size: " << state.size() << std::endl; 
	}

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ccsds */
} /* namespace gr */

