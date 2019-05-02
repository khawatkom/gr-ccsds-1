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

#ifndef INCLUDED_CCSDS_ENCODELDPC_IMPL_H
#define INCLUDED_CCSDS_ENCODELDPC_IMPL_H

#include <ccsds/encodeLDPC.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <gnuradio/blocks/pack_k_bits.h>
using namespace std;
namespace gr {
  namespace ccsds {

    class encodeLDPC_impl : public encodeLDPC
    {
     private:
      std::string d_out_tag_key;
      std::string d_file;
      int d_code_type;
      bool d_verbose;
      vector < vector<unsigned char> > G;
      int K;
      int N;
      int P;
      int c;
      int m;
      int M;
      uint64_t d_tag_position;
      blocks::kernel::pack_k_bits *d_pack;
      void encodeAR4JA(std::vector<unsigned char> &input);
      void encodeFullC2(std::vector<unsigned char> &input);
      void encodeC2(std::vector<unsigned char> &input);
      void encode(std::vector<unsigned char> &input);
      //void encodeAR4JA_R12_K1024(std::vector<unsigned char> &input);
      void (encodeLDPC_impl::*ldpcEncode)(std::vector<unsigned char> &input);
     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      encodeLDPC_impl(std::string circulant_file, int code_type,
                      std::string out_tag_key, std::string len_tag_key, int verbose);
      ~encodeLDPC_impl();
      void generatorMatrix();
      void leftrotate(string &s, int d);
      void rightrotate(string &s, int d);

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_ENCODELDPC_IMPL_H */

