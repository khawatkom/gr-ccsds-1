/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
#include "extractCADU_impl.h"
#include <gnuradio/digital/correlate_access_code_tag_bb.h>
#include <ccsds/processCADU.h>

namespace gr {
  namespace ccsds {

    extractCADU::sptr
    extractCADU::make(int frameLength, std::string syncWord, int tolerance,int descramble, int rs, int intDepth)
    {
      return gnuradio::get_initial_sptr
        (new extractCADU_impl(frameLength, syncWord, tolerance , descramble, rs, intDepth));
    }

    /*
     * The private constructor
     */
    extractCADU_impl::extractCADU_impl(int frameLength, std::string syncWord, int tolerance,int descramble, int rs, int intDepth)
      : gr::hier_block2("extractCADU",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(0, 0, 0))
    {
      //Registering output port
      message_port_register_out(pmt::mp("out"));
      
      //Correlate access code block
      gr::digital::correlate_access_code_tag_bb::sptr
	correlator(gr::digital::correlate_access_code_tag_bb::make("00011010110011111111110000011101",tolerance,"snyc"));
      ccsds::processCADU::sptr caduExtractor(ccsds::processCADU::make(frameLength, descramble,rs,intDepth,"sync"));

      connect(self(), 0, correlator, 0);
      connect(correlator,0,caduExtractor,0);
      msg_connect(caduExtractor,"out",self(),"out");
      //connect(caduExtractor,0,self(),0);
      //connect(d_lastblock, 0, self(), 0);
    }

    /*
     * Our virtual destructor.
     */
    extractCADU_impl::~extractCADU_impl()
    {
    }


  } /* namespace ccsds */
} /* namespace gr */

