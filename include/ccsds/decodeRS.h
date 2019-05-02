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


#ifndef INCLUDED_CCSDS_DECODERS_H
#define INCLUDED_CCSDS_DECODERS_H

#include <ccsds/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief <+description of block+>
     * \ingroup ccsds
     *
     */
    class CCSDS_API decodeRS : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<decodeRS> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ccsds::decodeRS.
       *
       * To avoid accidental use of raw pointers, ccsds::decodeRS's
       * constructor is in a private implementation
       * class. ccsds::decodeRS::make is the public interface for
       * creating new instances.
       */
      static sptr make(int code, int intDepth, int pass_all, int fert, int wert);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_DECODERS_H */

