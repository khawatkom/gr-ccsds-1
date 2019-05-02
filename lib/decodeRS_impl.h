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

#ifndef INCLUDED_CCSDS_DECODERS_IMPL_H
#define INCLUDED_CCSDS_DECODERS_IMPL_H

#include <ccsds/decodeRS.h>
#include "fec/rs/reedsolomon.h"

namespace gr {
  namespace ccsds {

    class decodeRS_impl : public decodeRS
    {
    private:
        int d_basis;
        int d_type;
        int d_intDepth;
        bool d_pass_all;
        bool d_fert;
        uint64_t d_rx_cnt;
        uint64_t d_valid_cnt;
        bool d_wert;
        int d_wer;
        uint64_t d_rx_wer_cnt;
        uint64_t d_wrong_wer_cnt;

     public:
      decodeRS_impl(int code, int intDepth, int pass_all, int fert, int wert);
      ~decodeRS_impl();
      void rsdecode(pmt::pmt_t pmt_msg);
      bool crc_ccitt(std::vector<unsigned char> frame);


      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_DECODERS_IMPL_H */

