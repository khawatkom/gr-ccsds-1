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

#ifndef INCLUDED_CCSDS_GENERATECADU_IMPL_H
#define INCLUDED_CCSDS_GENERATECADU_IMPL_H

#include <ccsds/generateCADU.h>
#include <vector>
#include <algorithm>
namespace gr {
  namespace ccsds {

    class generateCADU_impl : public generateCADU
    {
     private:
      int d_frameLength;
      bool d_scramble;
      bool d_rs;
      bool d_basis;
      int d_intDepth;
      std::vector<unsigned char> d_sync;

     public:
      unsigned char parse_hex(char c);
      std::vector<unsigned char> parse_string(const std::string & s);
      generateCADU_impl(int frameLength, std::string sync, int scramble, int rs, int intDepth);
      ~generateCADU_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_GENERATECADU_IMPL_H */

