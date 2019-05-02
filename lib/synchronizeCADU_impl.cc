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
#include "synchronizeCADU_impl.h"
#include <gnuradio/digital/correlate_access_code_tag_bb.h>
namespace gr {
  namespace ccsds {

    synchronizeCADU::sptr
    synchronizeCADU::make(std::string syncWord, int tolerance, std::string tagName)
    {
      return gnuradio::get_initial_sptr
        (new synchronizeCADU_impl(syncWord, tolerance, tagName));
    }

    /*
     * The private constructor
     */
    synchronizeCADU_impl::synchronizeCADU_impl(std::string syncWord, int tolerance, std::string tagName)
      : gr::hier_block2("synchronizeCADU",
			gr::io_signature::make(1, 1, sizeof(unsigned char)),
			gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      
      //Correlate access code block
      syncWord = hex_str_to_bin_str(syncWord);
      //"00011010110011111111110000011101"
      gr::digital::correlate_access_code_tag_bb::sptr
	correlator(gr::digital::correlate_access_code_tag_bb::make(syncWord,tolerance,tagName));
      connect(self(), 0, correlator, 0);
      // connect other blocks
      connect(correlator, 0, self(), 0);
    }

    /*
     * Our virtual destructor.
     */
    const char*
    synchronizeCADU_impl::hex_char_to_bin(char c)
    {
      // TODO handle default / error
      switch(toupper(c))
	{
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
	}
    }

    std::string
    synchronizeCADU_impl::hex_str_to_bin_str(const std::string& hex)
    {
      // TODO use a loop from <algorithm> or smth
      std::string bin;
      for(unsigned i = 0; i != hex.length(); ++i)
	bin += hex_char_to_bin(hex[i]);
      return bin;
    }
    synchronizeCADU_impl::~synchronizeCADU_impl()
    {
    }


  } /* namespace ccsds */
} /* namespace gr */

