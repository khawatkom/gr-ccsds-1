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
#include "encodeLDPC_impl.h"

namespace gr {
  namespace ccsds {

    encodeLDPC::sptr
    encodeLDPC::make(std::string circulant_file, int code_type, std::string out_tag_key, std::string len_tag_key, int verbose)
    {
      return gnuradio::get_initial_sptr
        (new encodeLDPC_impl(circulant_file,code_type,out_tag_key,len_tag_key,verbose));
    }

    /*
     * The private constructor
     */
    encodeLDPC_impl::encodeLDPC_impl(std::string circulant_file, int code_type,
                                     std::string out_tag_key, std::string len_tag_key, int verbose)
      : gr::tagged_stream_block("encodeLDPC",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)), len_tag_key),
        d_out_tag_key(out_tag_key),d_tag_position(0), d_file(circulant_file),d_code_type(code_type),d_verbose(verbose),c(0)
    {
        //Pack bits into bytes
        d_pack = new blocks::kernel::pack_k_bits(8);

        //Select encoder version
        switch(d_code_type)
        {
        case 0: //LDPC(8176,7154)
            //d_file = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/gmini/C2.txt";
            //d_file = "/ldpc/gmini/C2.txt";
            ldpcEncode = &encodeLDPC_impl::encodeFullC2;
            N = 8176;
            K = 7154;
            P = 1022;
            c = 14;
            m = 511;
            break;
        case 1: //LDPC(8160,7136)
            //d_file = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/gmini/C2.txt";
            //d_file = "./fec/ldpc/gmini/C2.txt";
            ldpcEncode = &encodeLDPC_impl::encodeC2;
            N = 8160;
            K = 7136;
            P = 1022;
            c = 14;
            m = 511;
            break;
        case 2: //AR4JA,r=1_2,k=1024 LDPC(2048,1024)
            //d_file = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/gmini/AR4JA_r12_k1024n.g";
            //d_file = "./fec/ldpc/gmini/AR4JA_r12_k1024n.g";
            ldpcEncode = &encodeLDPC_impl::encodeAR4JA;
            N = 2048;
            K = 1024;
            P = 1024;
            c = 8;
            m = 128;
            M = 512;
            break;
        case 3: //AR4JA,r=2_3,k=1024 LDPC(1536,1024)
            ldpcEncode = &encodeLDPC_impl::encodeAR4JA;
            N = 1536;
            K = 1024;
            P = N - K;
            c = 16;
            m = 64;
            M = 256;
            break;
        case 4: //AR4JA,r=4_5,k=1024 LDPC(1280,1024)
            ldpcEncode = &encodeLDPC_impl::encodeAR4JA;
            N = 1280;
            K = 1024;
            P = N - K;
            c = 32;
            m = 32;
            M = 128;
            break;
        }

        std::cout << "Generator matrix initialized" << std::endl;
        std::cout << "N : " << N << std::endl;
        std::cout << "K : " << K << std::endl;
        std::cout << "P : " << P << std::endl;

        //Initialize generator matrix
        generatorMatrix();

    }
    void
    encodeLDPC_impl::encodeFullC2(std::vector<unsigned char> &input)
    {
        //Initializing parity bits to 0x00
        std::vector<unsigned char> parity(P, 0x00);

        //Generating parity bits
        for(int i = 0; i < (input.size()); i++)
        {
            if(input.at(i))//input.at(i)
            {
                std::transform(parity.begin(), parity.end(), G.at(i).begin(),
                               parity.begin(), std::bit_xor<unsigned char>());
            }
        }

        //Appending parity bits to the original input
        input.insert(input.end(), parity.begin(), parity.end());
        //std::cout << "INSIDE C2 FUNCTION. Code Type : " << d_code_type << std::endl;
    }

    void encodeLDPC_impl::encodeC2(std::vector<unsigned char> &input)
    {
        std::vector<unsigned char> buffer = input;

        //Prepending 18 zeroes
        std::vector<unsigned char> prep(18,0x00);
        prep.insert(prep.end(),buffer.begin(),buffer.end());
        //std::cout << "Size of prep : " << prep.size() << std::endl;

        //LDPC Encoding
        //encode(input);

        //Initializing parity bits to 0x00
        std::vector<unsigned char> parity(P, 0x00);

        //Generating parity bits
        for(int i = 0; i < (prep.size()); i++)
        {
            if(prep.at(i))//input.at(i)
            {
                std::transform(parity.begin(), parity.end(), G.at(i).begin(),
                               parity.begin(), std::bit_xor<unsigned char>());
            }
        }
        //Remove the 18 prepend bits
        prep.erase(prep.begin(),prep.begin() + 18);

        //Insert parity bits
        input.insert(input.end(), parity.begin(), parity.end());
        //Append two bits;
        input.push_back(0x00);
        input.push_back(0x00);
    }


    void
    encodeLDPC_impl::encodeAR4JA(std::vector<unsigned char> &input)
    {
        encode(input);
    }

    void
    encodeLDPC_impl::encode(std::vector<unsigned char> &input)
    {
        //Initializing parity bits to 0x00
        std::vector<unsigned char> parity(P, 0x00);

        //Generating parity bits
        for(int i = 0; i < (K); i++)
        {
            if(input.at(i))//input.at(i)
            {
                std::transform(parity.begin(), parity.end(), G.at(i).begin(),
                               parity.begin(), std::bit_xor<unsigned char>());
            }
        }

        //Appending parity bits to the original input
        input.insert(input.end(), parity.begin(), parity.end());
        //std::cout << "INSIDE C2 FUNCTION. Code Type : " << d_code_type << std::endl;
    }

    void
    encodeLDPC_impl::leftrotate(string &s, int d)
    {
        reverse(s.begin(), s.begin()+d);
        reverse(s.begin()+d, s.end());
        reverse(s.begin(), s.end());
    }

    // In-place rotates s towards right by d
    void
    encodeLDPC_impl::rightrotate(string &s, int d)
    {
       leftrotate(s, s.length()-d);
    }


    void
    encodeLDPC_impl::generatorMatrix()
    {
        //Reading first circulants rows of generator matrix
        std::ifstream infile;
        infile.open(d_file);
        if (!infile) {
            cerr << "Unable to open circulant file : " << d_file << std::endl;
            exit(1);   // call system to stop
        }
        std::string line;
        std::vector<std::string> circulants;
        while (std::getline(infile, line))
        {
            circulants.push_back(line);
        }
        //std::cout << "Size of circulant before circulation : " << circulants.size() << std::endl;
        /*
        const int numCirculants   = 14;
        const int numCirculations = 511;
        const int lenCirculants   = 511*2;*/

        int numCirculants   = c;
        int numCirculations = m;
        int lenCirculants   = P;

        //The generator matrix
        std::vector<std::string> genMtx;

        for(int i = 0; i < numCirculants; i++)
        {
            std::string curCirculant = circulants.at(i);

            std::string b1 = curCirculant.substr(0,m);
            std::string b2 = curCirculant.substr(m,m);
            //std::cout << "Size of b1 : " << b1.length() << std::endl;
            //std::cout << "Size of b2 : " << b2.length() << std::endl;

            for(int j = 0; j < numCirculations; j++)
            {
                std::string b = b1 + b2;
                //genMtx.push_back(curCirculant);
                genMtx.push_back(b);
                rightrotate(b1,1);
                rightrotate(b2,1);
                //genMtx.push_back(curCirculant);
                //rightrotate(curCirculant,1);
//                if ((j < 2) && i < 1)
//                    std::cout << b1 << std::endl;
            }
        }
        //std::cout << "Size of circulant after circulation : " << genMtx.size() << std::endl;

        std::vector<unsigned char> row;
        for(int k = 0; k < numCirculants*numCirculations; k++)
        {

            for(int i = 0; i < lenCirculants; i++)
            {

                row.push_back((genMtx.at(k)[i] == '1') ? 0x01 : 0x00);

            }
            G.push_back(row);
            row.clear();
        }

    }

    /*
     * Our virtual destructor.
     */
    encodeLDPC_impl::~encodeLDPC_impl()
    {
    }

    int
    encodeLDPC_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = N/8;
      return noutput_items ;
    }

    int
    encodeLDPC_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      //Copying from the input array to a vector
      std::vector<unsigned char> input;
      input.insert(input.end(), in, in +  K);

      //Encoding ldpc codeword
      (*this.*ldpcEncode)(input);

      //Pack and transmit ldpc codeword
      d_pack->pack(out, input.data(), N/8);

      //Insert tag
      add_item_tag(0, d_tag_position,pmt::string_to_symbol(d_out_tag_key), pmt::from_long(N/8));
      d_tag_position += N/8;
      input.clear();
      // Tell runtime system how many output items we produced.
      return N/8;
    }

  } /* namespace ccsds */
} /* namespace gr */

