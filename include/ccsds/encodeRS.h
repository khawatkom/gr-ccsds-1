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


#ifndef INCLUDED_CCSDS_ENCODERS_H
#define INCLUDED_CCSDS_ENCODERS_H

#include <ccsds/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief <+description of block+>
     * \ingroup ccsds
     *
     */
    class CCSDS_API encodeRS : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<encodeRS> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ccsds::encodeRS.
       *
       * To avoid accidental use of raw pointers, ccsds::encodeRS's
       * constructor is in a private implementation
       * class. ccsds::encodeRS::make is the public interface for
       * creating new instances.
       */
      static sptr make(int code, int cbSize, int intDepth, std::string out_tag_key, std::string len_tag_key);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_ENCODERS_H */

