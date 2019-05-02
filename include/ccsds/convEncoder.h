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


#ifndef INCLUDED_CCSDS_CONVENCODER_H
#define INCLUDED_CCSDS_CONVENCODER_H

#include <ccsds/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief <+description of block+>
     * \ingroup ccsds
     *
     */
    class CCSDS_API convEncoder : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<convEncoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ccsds::convEncoder.
       *
       * To avoid accidental use of raw pointers, ccsds::convEncoder's
       * constructor is in a private implementation
       * class. ccsds::convEncoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(int invert);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CONVENCODER_H */

