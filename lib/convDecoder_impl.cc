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
#include "convDecoder_impl.h"
#include <gnuradio/fec/decoder.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/blocks/add_const_ff.h>
#include <gnuradio/blocks/float_to_uchar.h>
#include <iostream>
namespace gr {
  namespace ccsds {

    convDecoder::sptr
    convDecoder::make(int frame_size, std::vector<int> polys, int mode, int pad)
    {
      return gnuradio::get_initial_sptr
        (new convDecoder_impl(frame_size, polys, mode, pad));
    }

    /*
     * The private constructor
     */
    convDecoder_impl::convDecoder_impl(int frame_size, std::vector<int> polys, int mode, int pad)
      : gr::hier_block2("convDecoder",
              gr::io_signature::make(1, 1, sizeof(float)),
			gr::io_signature::make(1, 1, sizeof(unsigned char))),
	tracking(0)
    {
      //Creating a decoder variable
      int k = 7;
      int rate = 2;
      //set_history(12); not possible
      bool d_pad = (pad == 1) ? true : false;
      cc_mode_t d_mode = get_mode(mode);
      gr::fec::code::cc_decoder::sptr decoder_variable(gr::fec::code::cc_decoder::make(frame_size,k,rate,polys,0,-1,d_mode,d_pad));
      /*if (tracking == 0){
	std::cout << "input  type : " << decoder_variable->get_input_conversion() << std::endl;
	std::cout << "output type : " << decoder_variable->get_output_conversion() << std::endl;
	std::cout << "input size  : " << decoder_variable->get_input_item_size() << std::endl;	
      }
      tracking +=1;*/
      //Creating soft-decision converters
      gr::blocks::multiply_const_ff::sptr multiplier(gr::blocks::multiply_const_ff::make(48.0));
      gr::blocks::add_const_ff::sptr adder(gr::blocks::add_const_ff::make(128.0));
      gr::blocks::float_to_uchar::sptr float_to_soft(gr::blocks::float_to_uchar::make());
      //Creating a deployment variable
      gr::fec::decoder::sptr decoder_deployment(gr::fec::decoder::make(decoder_variable,sizeof(unsigned char),sizeof(unsigned char)));
      
      //Making connections
      connect(self() , 0, multiplier , 0);
      connect(multiplier, 0 , adder, 0);
      connect(adder , 0, float_to_soft, 0);
      connect(float_to_soft , 0 , decoder_deployment , 0);
      connect(decoder_deployment , 0 , self() , 0);
    }

   cc_mode_t convDecoder_impl::get_mode(int mode)
   {
     switch(mode)
       {
       case 0:
	 return CC_STREAMING;
       case 1:
	 return CC_TERMINATED;
       case 2:
	 return CC_TRUNCATED;
       case 3:
	 return CC_TAILBITING;
       default:
	 return CC_STREAMING;
       }
   }

    /*
     * Our virtual destructor.
     */
    convDecoder_impl::~convDecoder_impl()
    {
    }


  } /* namespace ccsds */
} /* namespace gr */

