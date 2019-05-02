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

#ifndef INCLUDED_CCSDS_SYNCHRONIZECADUSOFT_IMPL_H
#define INCLUDED_CCSDS_SYNCHRONIZECADUSOFT_IMPL_H

#include <ccsds/synchronizeCADUSoft.h>
enum state{SEARCH, LOCK};
enum cc {CC_1_2, CC_2_3, CC_3_4,CC_5_6, CC_7_8};
namespace gr {
  namespace ccsds {

    class synchronizeCADUSoft_impl : public synchronizeCADUSoft
    {
     private:
        std::string d_sync;
        int d_stl;
        int d_lts;
        int d_slip;
        int d_frame_size;
        int d_scramble;
        int d_fec;
        std::string d_tag_name;

        uint64_t d_access_code_0;
        uint64_t d_access_code_1;
        uint64_t d_access_code_2;
        uint64_t d_access_code_3;
        std::vector<uint64_t> d_wrong_bits;
        std::vector<uint64_t> d_nwrong;

      unsigned long long d_access_code;	// access code to locate start of packet
                                        //   access code is left justified in the word
      unsigned long long d_data_reg;	// used to look for access_code
      unsigned long long d_mask;	// masks access_code bits (top N bits are set where
                                        //   N is the number of bits in the access code)
      unsigned int d_threshold; 	// how many bits may be wrong in sync vector
      unsigned int d_len;               // the length of the access code

      pmt::pmt_t d_key, d_me; //d_key is the tag name, d_me is the block name + unique ID
      const char* hex_char_to_bin(char c);
      std::string hex_str_to_bin_str(const std::string& hex);
      gr::thread::mutex d_mutex_access_code;

      uint64_t abs_out_sample_cnt_last;
      uint64_t abs_out_sample_cnt_now;
      uint64_t d_consec_sync;
      uint64_t d_consec_loss;
      int d_ambiguity;
      state d_state;

      //Handling forward error correction
      bool d_invert_g1, d_invert_g2;
      std::string convEncode(std::string in, bool invG1, bool invG2, int code);

     public:
      synchronizeCADUSoft_impl(std::string sync, int stl, int lts, int slip, int frame_size, int fec, int invCC, std::string tag_name);
      ~synchronizeCADUSoft_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
      bool set_access_code(const std::string &access_code);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_SYNCHRONIZECADUSOFT_IMPL_H */

