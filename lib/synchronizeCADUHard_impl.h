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

#ifndef INCLUDED_CCSDS_SYNCHRONIZECADUHARD_IMPL_H
#define INCLUDED_CCSDS_SYNCHRONIZECADUHARD_IMPL_H

#include <ccsds/synchronizeCADUHard.h>
enum state{SEARCH, LOCK};
namespace gr {
  namespace ccsds {

    class synchronizeCADUHard_impl : public synchronizeCADUHard
    {
     private:
      std::string d_sync;
      int d_stl;
      int d_lts; 
      int d_tin; 
      int d_ambiguity; 
      int d_frame_size; 
      bool d_verbose;
      uint64_t d_access_code_0;
      uint64_t d_access_code_1;
      uint64_t d_access_code_2;
      uint64_t d_access_code_3;
      std::vector<uint64_t> d_wrong_bits;	
      std::vector<uint64_t> d_nwrong;
      uint64_t d_data_reg;	// used to look for access_code
      uint64_t d_mask;	// masks access_code bits (top N bits are set where
                                        //   N is the number of bits in the access code)
      unsigned int d_threshold; 	// how many bits may be wrong in sync vector
      unsigned int d_len;               // the length of the access code

      pmt::pmt_t d_key, d_me; //d_key is the tag name, d_me is the block name + unique ID
      const char* hex_char_to_bin(char c);
      std::string hex_str_to_bin_str(const std::string& hex);
      void add_tag(int id, uint64_t pos, pmt::pmt_t key);
      bool set_access_code(const std::string &access_code);

      uint64_t abs_out_sample_cnt_last;
      uint64_t abs_out_sample_cnt_now;
      uint64_t d_consec_sync;
      uint64_t d_consec_loss;
      state d_state;
     public:
      synchronizeCADUHard_impl(std::string sync, int stl, int lts, int tin, int ambiguity, int frame_size, int verbose);
      ~synchronizeCADUHard_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_SYNCHRONIZECADUHARD_IMPL_H */

