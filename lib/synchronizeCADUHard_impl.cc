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
#include "synchronizeCADUHard_impl.h"
#include <stdexcept>
#include <boost/format.hpp>
#include <volk/volk.h>
#include <gnuradio/math.h>
#include <cstdio>
#include <iostream>
#include <algorithm>
#define VERBOSE 0

namespace gr {
  namespace ccsds {

    synchronizeCADUHard::sptr
    synchronizeCADUHard::make(std::string sync, int stl, int lts, int tin, int ambiguity, int frame_size, int verbose)
    {
      return gnuradio::get_initial_sptr
        (new synchronizeCADUHard_impl(sync, stl, lts, tin, ambiguity, frame_size, verbose));
    }

    /*
     * The private constructor
     */
    synchronizeCADUHard_impl::synchronizeCADUHard_impl(std::string sync, int stl, int lts, int tin, int ambiguity, int frame_size, int verbose)
      : gr::sync_block("synchronizeCADUHard",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
        d_sync(sync), d_stl(stl), d_lts(lts), d_tin(tin),
        d_ambiguity(ambiguity), d_frame_size(frame_size), d_verbose(verbose > 0),
        d_threshold(0),d_consec_sync(0),d_state(SEARCH),d_access_code_0(0),
        d_access_code_1(0),d_access_code_2(0),d_access_code_3(0)

    {

        std::string sync_word = hex_str_to_bin_str(sync);
        //std::cout << "Length of sync word : " << sync_word.length();
        if(!set_access_code(sync_word)) {
            throw std::out_of_range ("access_code is > 64 bits");
        }

        std::stringstream str;
        str << name() << unique_id();
        d_me = pmt::string_to_symbol(str.str());
        d_key = pmt::string_to_symbol("sync");
        //std::cout << "Constructor of synchronizeCADU block is called : " << unique_id() << std::endl;
    }

    /*
     * Our virtual destructor.
     */
    synchronizeCADUHard_impl::~synchronizeCADUHard_impl()
    {
    }

    bool
    synchronizeCADUHard_impl::set_access_code(
            const std::string &access_code)
    {
        d_len = access_code.length();	// # of bytes in string
        if(d_len > 64)
            return false;

        // set len bottom bits to 1.
        d_mask = ((~0ULL) >> (64 - d_len));
	//Setting access code for 90 and 270 ambiguities
        

        std::string access_code_90_270 = access_code;
        //Swap I and Q 
        unsigned temp1, temp2;
        for(unsigned j = 0; j < d_len; j+=2){
                //temp1 = access_code[j];
                //temp2 = access_code[j + 1];
		access_code_90_270[j] = access_code[j + 1];
        	access_code_90_270[j + 1] = access_code[j];
        }

        for(unsigned i=0; i < d_len; i++){
            d_access_code_0 = (d_access_code_0 << 1) | (access_code[i] & 1); //0   degrees
            d_access_code_1 = (d_access_code_1 << 1) | ((access_code_90_270[i] & 1)^ ((i%2 == 0)? 1 : 0)); //90  degrees
            d_access_code_2 = (d_access_code_2 << 1) | ((access_code[i] & 1)^1); //180 degrees
            d_access_code_3 = (d_access_code_3 << 1) | ((access_code_90_270[i] & 1)^ ((i%2 == 0)? 0 : 1)); //270 degrees
        }

        if(VERBOSE) {
            //std::cerr << "Access code: " << std::hex << d_access_code << std::dec << std::endl;
            //std::cerr << "Mask: " << std::hex << d_mask << std::dec << std::endl;
        }

        return true;
    }
    inline int findlowest(int a, int b, int c, int d)
    {
        int of_a_b = a < b ? a : b;
        int of_c_d = c < d ? c : d;
        return of_a_b < of_c_d ? of_a_b : of_c_d;
    }
    const char*
    synchronizeCADUHard_impl::hex_char_to_bin(char c)
    {
        // TODO handle default / error
        switch(toupper(c))
        {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        }
    }

    std::string
    synchronizeCADUHard_impl::hex_str_to_bin_str(const std::string& hex)
    {
        // TODO use a loop from <algorithm> or smth
        std::string bin;
        for(unsigned i = 0; i != hex.length(); ++i)
            bin += hex_char_to_bin(hex[i]);
        return bin;
    }

    void
    synchronizeCADUHard_impl::add_tag(int id, uint64_t pos, pmt::pmt_t key)
    {
        uint64_t nwrong = 3;
        add_item_tag(id, //stream ID
                     pos, //sample
                     key,      //frame info
                     pmt::from_long(nwrong), //data (number wrong)
                     d_me        //block src id
                     );
    }

    int
    synchronizeCADUHard_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      uint64_t abs_out_sample_cnt = nitems_written(0);
      // = nitems_written(0);
      int interval;

      for(int i = 0; i < noutput_items; i++) {
        out[i] = in[i];
        abs_out_sample_cnt_now = abs_out_sample_cnt + i;


        // compute hamming distance between desired access code and current data
        //std::vector<uint64_t> wrong_bits;
        //std::vector<uint64_t> nwrong = d_threshold+1;

        //Initilizing synchronizer variables
        uint64_t nwrong_0 = d_threshold+1;
        uint64_t nwrong_1 = d_threshold+1;
        uint64_t nwrong_2 = d_threshold+1;
        uint64_t nwrong_3 = d_threshold+1;

        uint64_t wrong_bits_0 = (d_data_reg ^ d_access_code_0) & d_mask;
        uint64_t wrong_bits_1 = (d_data_reg ^ d_access_code_1) & d_mask;
        uint64_t wrong_bits_2 = (d_data_reg ^ d_access_code_2) & d_mask;
        uint64_t wrong_bits_3 = (d_data_reg ^ d_access_code_3) & d_mask;




        volk_64u_popcnt(&nwrong_0, wrong_bits_0);
        volk_64u_popcnt(&nwrong_1, wrong_bits_1);
        volk_64u_popcnt(&nwrong_2, wrong_bits_2);
        volk_64u_popcnt(&nwrong_3, wrong_bits_3);



        //Find minimum element in d_nwrong vector
        int n_wrong_min = findlowest(nwrong_0,nwrong_1,nwrong_2,nwrong_3);//std::min(  nwrong_0,nwrong_1,nwrong_2,nwrong_3 );


        // shift in new data
        d_data_reg = (d_data_reg << 1) | (in[i] & 0x1);
        if(n_wrong_min <= d_threshold) {
            if(nwrong_0 == n_wrong_min)
            {
                d_ambiguity = 0;
            }
            else if (nwrong_1 == n_wrong_min)
            {
                d_ambiguity = 90;
            }
            else if (nwrong_2 == n_wrong_min)
            {
                d_ambiguity = 180;
            }
            else if (nwrong_3 == n_wrong_min)
            {
                d_ambiguity = 270;
            }
            else{
                d_ambiguity = 0;
            }
            //std::cout << "Ambiguity : " << d_ambiguity << std::endl;



           interval = abs_out_sample_cnt_now - abs_out_sample_cnt_last;
           if( (interval/d_frame_size) == 1)
           {
               d_consec_sync += 1;
               d_consec_loss  = 0;
           }
           else
           {
               d_consec_sync =  0;
               d_consec_loss += 1;
           }

           if ((d_state == SEARCH) && (d_consec_sync >= d_stl))
           {
               d_state = LOCK;
               if(d_verbose)
               std::cout <<"State Change : SEARCH TO LOCK" <<std::endl;
           }
           if ((d_state == LOCK) && (d_consec_loss >= d_lts))
           {
               d_state = SEARCH;
               if(d_verbose)
               std::cout <<"State Change : LOCK TO SEARCH" <<std::endl;
           }

           if(d_verbose)
           {
           //std::cout << "Consecutive Syncs : " << d_consec_sync << std::endl;
           //std::cout << "Consecutive Loss  : " << d_consec_loss << std::endl;
           }
           abs_out_sample_cnt_last = abs_out_sample_cnt + i;
        }

        switch(d_state)
        {
        case SEARCH:
            d_threshold = 0;
            //if ((abs_out_sample_cnt_now - abs_out_sample_cnt_last) % d_frame_size == 0)
             //   add_tag(0,abs_out_sample_cnt_now,d_key);
            break;

        case LOCK:
            d_threshold = d_tin;
            //std::cout <<"Current State : LOCK" <<std::endl;
            //std::cout <<"Element : " << element << std::endl;
            if ((abs_out_sample_cnt_now - abs_out_sample_cnt_last) % d_frame_size == 0)
            {
                //Creating key tag for ambiguity resolution

                add_item_tag(0,abs_out_sample_cnt_now,d_key,pmt::from_long(d_ambiguity),d_me);

            }
            break;
        }

      }
      abs_out_sample_cnt_now = 0;
      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ccsds */
} /* namespace gr */

