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
#include "synchronizeCADUSoft_impl.h"
#include <stdexcept>
#include <boost/format.hpp>
#include <volk/volk.h>
#include <gnuradio/math.h>
#include <cstdio>
#include <iostream>
#include <string>
#define VERBOSE 0
namespace gr {
namespace ccsds {

synchronizeCADUSoft::sptr
synchronizeCADUSoft::make(std::string sync, int stl, int lts, int slip, int frame_size,int fec, int invCC, std::string tag_name)
{
    return gnuradio::get_initial_sptr
            (new synchronizeCADUSoft_impl(sync,stl,lts,slip,frame_size,fec,invCC,tag_name));
}

/*
     * The private constructor
     */
synchronizeCADUSoft_impl::synchronizeCADUSoft_impl(std::string sync, int stl, int lts,
                                                   int slip, int frame_size,
                                                   int fec, int invCC,std::string tag_name)
    : gr::sync_block("synchronizeCADUSoft",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float))),
      d_data_reg(0), d_mask(0),
      d_len(0), d_sync(sync),d_stl(stl),
      d_lts(lts),d_slip(slip),
      d_frame_size(frame_size), d_fec(fec), d_tag_name(tag_name),
      d_ambiguity(0),d_threshold(0),
      d_consec_sync(0),d_state(SEARCH),d_access_code_0(0),
      d_access_code_1(0),d_access_code_2(0),d_access_code_3(0),
      d_invert_g1(invCC == 1), d_invert_g2(invCC == 2)
{
    std::string sync_word;
    switch(d_fec)
    {
    case 0:
       sync_word = hex_str_to_bin_str(d_sync);
        break;
    default:
        sync_word = convEncode(hex_str_to_bin_str(d_sync),d_invert_g1,d_invert_g2,d_fec);
        break;
    }


    //std::string test = convEncode(hex_str_to_bin_str(d_sync),d_invert_g1,d_invert_g2,1);
    std::cout << " sync word : " << sync_word << std::endl;
    std::cout << "Length of sync word : " << sync_word.length() << std::endl;
    if(!set_access_code(sync_word)) {
        throw std::out_of_range ("access_code is > 64 bits");
    }

    std::stringstream str;
    str << name() << unique_id();
    d_me = pmt::string_to_symbol(str.str());
    d_key = pmt::string_to_symbol(tag_name);
}

bool
synchronizeCADUSoft_impl::set_access_code(
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

std::string
synchronizeCADUSoft_impl::convEncode(std::string in, bool invG1, bool invG2,int code)
{
    std::vector<unsigned char> state, output;
    const int K = 7;
    for(int i = 0; i < K; i++)
        state.push_back(0x01);
    unsigned char bit0, bit1;
    for(int i = 0; i < in.length(); i++){
        state.erase(state.begin());
        //Insert new element
        state.push_back(in[i] == '1' ? 0x01 : 0x00);
        //Generate output bits
        bit0 = state.at(0) ^ state.at(3) ^ state.at(4) ^ state.at(5) ^ state.at(6); //G1 = 1111001 (Taken in reverse)
        bit1 = state.at(0) ^ state.at(1) ^ state.at(3) ^ state.at(4) ^ state.at(6); //G2 = 1011011 (Taken in reverse)
        //Inverting bits
        bit0 = invG1 ? ~bit0&0x01 : bit0;
        bit1 = invG2 ? ~bit1&0x01 : bit1;
        output.push_back(bit0);
        output.push_back(bit1);
    }
    std::cout << "Size of sync output buffer : " << output.size() << std::endl;
    std::string sync;
    for(int i = (K - 1)*2; i < output.size(); i ++)
        sync.push_back(output[i] == 0x01 ? '1' : '0');

    return sync;
}

inline int findlowest(int a, int b, int c, int d)
{
    int of_a_b = a < b ? a : b;
    int of_c_d = c < d ? c : d;
    return of_a_b < of_c_d ? of_a_b : of_c_d;
}

/*
     * Our virtual destructor.
     */
synchronizeCADUSoft_impl::~synchronizeCADUSoft_impl()
{
}

const char*
synchronizeCADUSoft_impl::hex_char_to_bin(char c)
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
synchronizeCADUSoft_impl::hex_str_to_bin_str(const std::string& hex)
{
    // TODO use a loop from <algorithm> or smth
    std::string bin;
    for(unsigned i = 0; i != hex.length(); ++i)
        bin += hex_char_to_bin(hex[i]);
    return bin;
}

static inline unsigned char
slice(float x)
{
    return x < 0 ? 0x00 : 0x01;
}

int
synchronizeCADUSoft_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
{
    gr::thread::scoped_lock l(d_mutex_access_code);

    const float *in = (const float*)input_items[0];
    float *out = (float*)output_items[0];

    uint64_t abs_out_sample_cnt = nitems_written(0);
    int interval;
    bool d_verbose = false;

    for(int i = 0; i < noutput_items; i++) {
        out[i] = in[i];
        abs_out_sample_cnt_now = abs_out_sample_cnt + i;

        //Initilizing synchronizer variables. Four versions of sync word are needed for ambiguity resolution.
        uint64_t nwrong_0 = d_threshold+1;
        uint64_t nwrong_1 = d_threshold+1;
        uint64_t nwrong_2 = d_threshold+1;
        uint64_t nwrong_3 = d_threshold+1;

        uint64_t wrong_bits_0 = (d_data_reg ^ d_access_code_0) & d_mask;
        uint64_t wrong_bits_1 = (d_data_reg ^ d_access_code_1) & d_mask;
        uint64_t wrong_bits_2 = (d_data_reg ^ d_access_code_2) & d_mask;
        uint64_t wrong_bits_3 = (d_data_reg ^ d_access_code_3) & d_mask;



        //Correlation by hamming distance
        volk_64u_popcnt(&nwrong_0, wrong_bits_0);
        volk_64u_popcnt(&nwrong_1, wrong_bits_1);
        volk_64u_popcnt(&nwrong_2, wrong_bits_2);
        volk_64u_popcnt(&nwrong_3, wrong_bits_3);



        //Find minimum element in d_nwrong vector
        int n_wrong_min = findlowest(nwrong_0,nwrong_1,nwrong_2,nwrong_3);


        /*
                // compute hamming distance between desired access code and current data
                uint64_t wrong_bits = 0;
                uint64_t nwrong = d_threshold+1;

                wrong_bits  = (d_data_reg ^ d_access_code) & d_mask;
                volk_64u_popcnt(&nwrong, wrong_bits);

                // shift in new data
                d_data_reg = (d_data_reg << 1) | (gr::branchless_binary_slicer(in[i]) & 0x1);
                if(nwrong <= d_threshold) {
                  GR_LOG_DEBUG(d_logger, boost::format("writing tag at sample %llu") % (abs_out_sample_cnt + i));
                  add_item_tag(0, //stream ID
                               abs_out_sample_cnt + i, //sample
                               d_key,      //frame info
                               pmt::from_long(nwrong), //data (number wrong)
                               d_me        //block src id
                               );
                }*/

        // shift in new data
        d_data_reg = (d_data_reg << 1) | (gr::branchless_binary_slicer(in[i]) & 0x1);
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
                std::cout << "In the default state" << std::endl;
            }
            //std::cout << "Ambiguity : " << d_ambiguity << std::endl;

            interval = abs_out_sample_cnt_now - abs_out_sample_cnt_last;
            if( (interval/(d_frame_size)) == 1)
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
             //std::cout <<"Current State : SEARCH" <<std::endl;
             d_threshold = 0;
             //if ((abs_out_sample_cnt_now - abs_out_sample_cnt_last) % d_frame_size == 0)
              //   add_tag(0,abs_out_sample_cnt_now,d_key);
             break;

         case LOCK:
             d_threshold = d_slip;
             //std::cout <<"Current State : LOCK" <<std::endl;
             //std::cout <<"Element : " << element << std::endl;
             if ((abs_out_sample_cnt_now - abs_out_sample_cnt_last) % (d_frame_size) == 0)
             {
                 //Creating key tag for ambiguity resolution

                 add_item_tag(0,abs_out_sample_cnt_now,d_key,pmt::from_long(d_ambiguity),d_me);

             }
             break;
         }

       }
       abs_out_sample_cnt_now = 0;



    return noutput_items;
}

} /* namespace ccsds */
} /* namespace gr */

