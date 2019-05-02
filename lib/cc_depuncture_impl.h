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

#ifndef INCLUDED_CCSDS_CC_DEPUNCTURE_IMPL_H
#define INCLUDED_CCSDS_CC_DEPUNCTURE_IMPL_H

#include <ccsds/cc_depuncture.h>

namespace gr {
  namespace ccsds {

    class cc_depuncture_impl : public cc_depuncture
    {
     private:
      int d_puncsize;
      int d_delay;
      int d_puncholes;
      int d_puncpat;
      char d_sym;

     public:
      cc_depuncture_impl(int puncsize, int puncpat, int delay, char symbol);
      ~cc_depuncture_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
      int fixed_rate_ninput_to_noutput(int ninput);
      int fixed_rate_noutput_to_ninput(int noutput);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CC_DEPUNCTURE_IMPL_H */

