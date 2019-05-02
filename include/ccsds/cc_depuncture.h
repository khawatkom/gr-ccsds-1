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


#ifndef INCLUDED_CCSDS_CC_DEPUNCTURE_H
#define INCLUDED_CCSDS_CC_DEPUNCTURE_H

#include <ccsds/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief <+description of block+>
     * \ingroup ccsds
     *
     */
    class CCSDS_API cc_depuncture : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<cc_depuncture> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ccsds::cc_depuncture.
       *
       * To avoid accidental use of raw pointers, ccsds::cc_depuncture's
       * constructor is in a private implementation
       * class. ccsds::cc_depuncture::make is the public interface for
       * creating new instances.
       */
      static sptr make(int puncsize, int puncpat, int delay, char symbol);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CC_DEPUNCTURE_H */

