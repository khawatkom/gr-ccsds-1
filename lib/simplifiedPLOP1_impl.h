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

#ifndef INCLUDED_CCSDS_SIMPLIFIEDPLOP1_IMPL_H
#define INCLUDED_CCSDS_SIMPLIFIEDPLOP1_IMPL_H

#include <ccsds/simplifiedPLOP1.h>
#include <stdint.h>
#include <string.h>
namespace gr {
  namespace ccsds {

    class simplifiedPLOP1_impl : public simplifiedPLOP1
    {
     private:
      uint8_t *idleSeq;
      uint8_t *acqSeq;
      int d_idleSeqLen;
      int d_acqSeqLen;

     public:
      simplifiedPLOP1_impl(int idleSeqLen, int acqSeqLen);
      ~simplifiedPLOP1_impl();
      void plop1(pmt::pmt_t pmt_msg);
      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_SIMPLIFIEDPLOP1_IMPL_H */

