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

#ifndef INCLUDED_CCSDS_DECODETURBO_IMPL_H
#define INCLUDED_CCSDS_DECODETURBO_IMPL_H

#include <ccsds/decodeTurbo.h>
extern "C"
{
	#include "fec/turbo/libconvcodes.h"
	#include "fec/turbo/libturbocodes.h"
}
#define MAX_COMPONENTS 4
namespace gr {
  namespace ccsds {

    class decodeTurbo_impl : public decodeTurbo
    {
     private:
      uint64_t d_count;
      int d_terminate;
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;
      int d_base;
      int d_octets;
      int d_code_type;
      int d_iterations;
      float d_sigma;
      bool d_update_sigma;

      float d_rate;
      int d_info_length;
      int d_encoded_length;

      int *d_pi;
      const char *d_forward_upper[MAX_COMPONENTS];
      const char *d_forward_lower[MAX_COMPONENTS];
      const char *d_backward;
      t_convcode d_code1;
      t_convcode d_code2;
      t_turbocode d_turbo;


      void pmt_in_callback(pmt::pmt_t msg);
      int puncturing(int k);

     public:
      decodeTurbo_impl(int base, int octets, int code_type, int iterations, float sigma, uint8_t update_sigma);
      ~decodeTurbo_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_DECODETURBO_IMPL_H */

