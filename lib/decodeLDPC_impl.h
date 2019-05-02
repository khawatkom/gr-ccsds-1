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

#ifndef INCLUDED_CCSDS_DECODELDPC_IMPL_H
#define INCLUDED_CCSDS_DECODELDPC_IMPL_H

#include <ccsds/decodeLDPC.h>
//#include "fec/ldpc/soft_decoder.h"
#include "fec/ldpc/checknode.h"
#include "fec/ldpc/variablenode.h"
#include "fec/ldpc/ldpc.h"
#include <cassert>
#include <gnuradio/blocks/pack_k_bits.h>
namespace gr {
  namespace ccsds {

    class decodeLDPC_impl : public decodeLDPC
    {
     private:
        int d_iterations;
        float d_sigma;
        int d_update_sigma;
        int d_code_type;
        bool d_pack;
        blocks::kernel::pack_k_bits *d_pack_8;
        int punct;
        string d_file;
        pmt::pmt_t d_in_port;
        pmt::pmt_t d_out_port;
        int** H; //Parity matrix
        //double *inputData;
        ldpc *ccsdsLDPC;
        std::vector<unsigned char> decodeFullC2(std::vector<float> softBits);
        std::vector<unsigned char> decodeExpC2(std::vector<float> softBits);
        std::vector<unsigned char> decodeAR4JA(std::vector<float> softBits);
        std::vector<unsigned char> (decodeLDPC_impl::*ldpcDecode)(std::vector<float> input);
        std::vector<unsigned char> slice(std::vector<float> logapp);

     public:
      decodeLDPC_impl(std::string alist_file, int code_type, int iterations, float sigma, int update_sigma,int pack);
      ~decodeLDPC_impl();
      void decode(pmt::pmt_t msg);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_DECODELDPC_IMPL_H */

