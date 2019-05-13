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

#ifndef INCLUDED_CCSDS_SYNCHRONIZECADUARGMAX_IMPL_H
#define INCLUDED_CCSDS_SYNCHRONIZECADUARGMAX_IMPL_H

#include <ccsds/synchronizeCADUArgMax.h>

namespace gr {
  namespace ccsds {

    class synchronizeCADUArgMax_impl : public synchronizeCADUArgMax
    {
     private:
        std::string d_sync;
        int d_frame_size;
        int d_mtxFunction;
        int d_ambiguity;
        int d_fec;
        bool d_verbose;
        std::string d_tag_name;


     public:
      synchronizeCADUArgMax_impl(std::string sync, int size, int mtxFunction, int ambiguity, int fec, int verbose, std::string tagName);
      ~synchronizeCADUArgMax_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_SYNCHRONIZECADUARGMAX_IMPL_H */

