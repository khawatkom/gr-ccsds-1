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

#ifndef INCLUDED_CCSDS_SYNCHRONIZECADU_IMPL_H
#define INCLUDED_CCSDS_SYNCHRONIZECADU_IMPL_H

#include <ccsds/synchronizeCADU.h>

namespace gr {
  namespace ccsds {

    class synchronizeCADU_impl : public synchronizeCADU
    {
     private:
      const char* hex_char_to_bin(char c);
      std::string hex_str_to_bin_str(const std::string& hex);

     public:
      synchronizeCADU_impl(std::string syncWord, int tolerance, std::string tagName);
      ~synchronizeCADU_impl();

      // Where all the action really happens
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_SYNCHRONIZECADU_IMPL_H */

