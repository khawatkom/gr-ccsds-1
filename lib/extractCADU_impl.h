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

#ifndef INCLUDED_CCSDS_EXTRACTCADU_IMPL_H
#define INCLUDED_CCSDS_EXTRACTCADU_IMPL_H

#include <ccsds/extractCADU.h>

namespace gr {
  namespace ccsds {

    class extractCADU_impl : public extractCADU
    {
     private:
      // Nothing to declare in this block.

     public:
      extractCADU_impl(int frameLength, std::string syncWord, int tolerance,int descramble, int rs, int intDepth);
      ~extractCADU_impl();

      // Where all the action really happens
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_EXTRACTCADU_IMPL_H */

