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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "synchronizeCADUArgMax_impl.h"

namespace gr {
  namespace ccsds {

    synchronizeCADUArgMax::sptr
    synchronizeCADUArgMax::make(std::string sync, int size, int mtxFunction, int ambiguity, int fec, int verbose, std::string tagName)
    {
      return gnuradio::get_initial_sptr
        (new synchronizeCADUArgMax_impl(sync, size, mtxFunction, ambiguity, fec, verbose, tagName));
    }

    /*
     * The private constructor
     */
    synchronizeCADUArgMax_impl::synchronizeCADUArgMax_impl(std::string sync, int size, int mtxFunction, int ambiguity,
                                                           int fec, int verbose, std::string tagName)
      : gr::sync_block("synchronizeCADUArgMax",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))),
        d_sync(sync),d_frame_size(size),d_mtxFunction(mtxFunction),d_ambiguity(ambiguity),d_fec(fec),
        d_verbose(verbose > 0),d_tag_name(tagName)
    {

    }

    /*
     * Our virtual destructor.
     */
    synchronizeCADUArgMax_impl::~synchronizeCADUArgMax_impl()
    {
    }

    int
    synchronizeCADUArgMax_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ccsds */
} /* namespace gr */

