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
#include "decodeLDPC_impl.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#define N 8176
#define K 7154
#define P 1022
using namespace std;
namespace gr {
  namespace ccsds {

    decodeLDPC::sptr
    decodeLDPC::make(std::string alist_file, int code_type,int iterations, float sigma, int update_sigma, int pack)
    {
      return gnuradio::get_initial_sptr
        (new decodeLDPC_impl(alist_file,code_type,iterations, sigma, update_sigma,pack));
    }

    /*
     * The private constructor
     */
    decodeLDPC_impl::decodeLDPC_impl(std::string alist_file, int code_type,int iterations, float sigma, int update_sigma,int pack)
      : gr::sync_block("decodeLDPC",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_iterations(iterations),d_sigma(sigma),d_update_sigma(update_sigma),
        d_file(alist_file),d_code_type(code_type), d_pack(pack)
    {
        //Initializing input and output ports
        d_in_port = pmt::mp("in");
        message_port_register_in(d_in_port);

        d_out_port = pmt::mp("out");
        message_port_register_out(d_out_port);

        //Setting callback
        set_msg_handler(d_in_port, boost::bind(&decodeLDPC_impl::decode, this ,_1) );

        //Packing bits into bytes
        d_pack_8 = new blocks::kernel::pack_k_bits(8);


        switch(code_type)
        {
        case 0:
            ldpcDecode  = &decodeLDPC_impl::decodeFullC2;
            //d_file = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/alist/C2_Alist.a";
            break;
        case 1:
            ldpcDecode = &decodeLDPC_impl::decodeExpC2;
            //d_file = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/alist/C2_Alist.a";
            break;
        case 2:
            ldpcDecode = &decodeLDPC_impl::decodeAR4JA;
            punct = 512;
            //d_file = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/alist/AR4JA_r12_k1024n.a";
            break;
        }

        //LDPC codec object
        ccsdsLDPC = new ldpc(d_file);

        //Printing properties of the LDPC codec
        std::cout << "LDPC Decoder Initialized .." << std::endl;
        std::cout << "Code rate  : " << ccsdsLDPC->getCodeRate() << std::endl;
        std::cout << "Check nodes: " << ccsdsLDPC->getnumCheckNodes() << std::endl;
        std::cout << "Variable nodes : " << ccsdsLDPC->getnumVarNodes() << std::endl;

        set_output_multiple(N);
    }
    std::vector<unsigned char>
    decodeLDPC_impl::decodeFullC2(std::vector<float> softBits)
    {
        std::vector<float> logapp = ccsdsLDPC->decode(softBits,d_iterations,d_sigma);
        std::vector<unsigned char> decodedBits = slice(logapp);

        return decodedBits;
    }

    std::vector<unsigned char>
    decodeLDPC_impl::decodeExpC2(std::vector<float> softBits)
    {
        // Removing the last two LLRs
        softBits.pop_back();
        softBits.pop_back();

        // Insert LLRs for the first 18 elements
        std::vector<float> codeword(18,-1000.00);
        codeword.insert(codeword.end(),softBits.begin(),softBits.end());

        // LDPC Decoding
        std::vector<float> logapp = ccsdsLDPC->decode(codeword,d_iterations,d_sigma);

        // Slicing log-APP ratios into hard bits
        std::vector<unsigned char> decodedBits = slice(logapp);

        // Remove the prefixed zero bits
        decodedBits.erase(decodedBits.begin(),decodedBits.begin() + 18);

        return decodedBits;
    }

    std::vector<unsigned char>
    decodeLDPC_impl::decodeAR4JA(std::vector<float> softBits)
    {
        for(int i = 0; i < punct; i++)
            softBits.push_back(0.0);
        std::vector<float> logapp = ccsdsLDPC->decode(softBits,d_iterations,d_sigma);
        std::vector<unsigned char> decodedBits = slice(logapp);
        return decodedBits;
    }


    std::vector<unsigned char>
    decodeLDPC_impl::slice(std::vector<float> logapp)
    {
        std::vector<unsigned char> bits;
        unsigned char bit;
        for(int i = 0; i < logapp.size(); i++)
            bits.push_back( (logapp[i] > 0) ? 0x01 : 0x00);
        return bits;
    }

    void
    decodeLDPC_impl::decode(pmt::pmt_t msg)
    {
        pmt::pmt_t meta(pmt::car(msg));
        pmt::pmt_t bits(pmt::cdr(msg));

        std::vector<float> softBits = pmt::f32vector_elements(bits);
        //std::cout << "Size of softBits vector : " << softBits.size() << std::endl;
        // LDPC Decoding
        std::vector<unsigned char> decodedBits = (*this.*ldpcDecode)(softBits);

        if(d_pack)
        {
            uint8_t *decoded_u8 = (uint8_t *)malloc(sizeof(uint8_t)*decodedBits.size()/8);
            for(int i=0; i<decodedBits.size()/8; i++)
            {
                decoded_u8[i] = 0;
                for(int j=0; j<8; j++)
                {
                    decoded_u8[i] |= decodedBits[i*8+j]?(0x80>>j):0;
                }
            }
            // Publishing data
            pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(decoded_u8,decodedBits.size()/8)));
            message_port_pub(d_out_port, pdu);
            free(decoded_u8);
        }
        else
        {
        // Publishing data
        pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(decodedBits.data(),decodedBits.size())));
                    message_port_pub(d_out_port, pdu);
        }
        decodedBits.clear();
        softBits.clear();

    }

    /*
     * Our virtual destructor.
     */
    decodeLDPC_impl::~decodeLDPC_impl()
    {
    }

    int
    decodeLDPC_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
//      const float *in = (const float *) input_items[0];
//      //<+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

//      std::vector<double> softBits;
//      for(int i = 0; i < N; i++)
//         softBits.push_back(in[i]);

      // LDPC Decoding
      //ldpc decoderLDPC =  ldpc(d_file);
      //std::vector<unsigned char> decodedBits = (*this.*ldpcDecode)(softBits);
      //std::vector<double> decodedBits = decoderLDPC.decode(softBits,d_iterations,d_sigma);
      // Do <+signal processing+>
      //softBits.clear();
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ccsds */
} /* namespace gr */

