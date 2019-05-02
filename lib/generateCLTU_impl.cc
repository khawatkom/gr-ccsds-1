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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "generateCLTU_impl.h"

namespace gr {
namespace ccsds {

generateCLTU::sptr
generateCLTU::make(int scramble)
{
    return gnuradio::get_initial_sptr
            (new generateCLTU_impl(scramble));
}

/*
     * The private constructor
     */
generateCLTU_impl::generateCLTU_impl(int scramble)
    : gr::sync_block("generateCLTU",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(0, 0, 0))
{
    d_scramble = (scramble == 1);
    message_port_register_in(pmt::mp("in"));
    message_port_register_out(pmt::mp("out"));
    set_msg_handler(
                pmt::mp("in"),
                boost::bind(&generateCLTU_impl::cltu, this, _1)
                );

    //Initializing CRC table
    crcTable = (unsigned char*)malloc(sizeof(unsigned char)*256);
    crcInit();
}

/*
     * Our virtual destructor.
     */
generateCLTU_impl::~generateCLTU_impl()
{
}

void
generateCLTU_impl::cltu(pmt::pmt_t pmt_msg)
{
    //Extracting message from pmt
    pmt::pmt_t msg = pmt::cdr(pmt_msg);
    size_t offset(0);
    unsigned char *msg_char = (unsigned char*)malloc(pmt::length(msg));
    std::vector<unsigned char> tc;
    memcpy(msg_char,pmt::uniform_vector_elements(msg, offset),pmt::length(msg));
    tc.insert(tc.end(), msg_char, msg_char + pmt::length(msg));

    //Optional scrambling
    if(d_scramble)  S.Scramble(tc);

    //TO DO : BCH encoding and testing
    bchEncoder(tc);

    //CLTU generation
    unsigned char start[] = {0xEB,0x90};
    std::vector<unsigned char> startSequence(start, start + sizeof(start)/sizeof(unsigned char));
    unsigned char tail[] = {0xC5,0xC5,0xC5,0xC5,0xC5,0xC5,0xC5,0x79};
    std::vector<unsigned char> tailSequence(tail, tail + sizeof(tail)/sizeof(unsigned char));

    tc.insert(tc.begin(),startSequence.begin(),startSequence.end());
    tc.insert(tc.end(),tailSequence.begin(),tailSequence.end());
    //Sending payload
    pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(tc.data(),tc.size())));
    message_port_pub(pmt::mp("out"), pdu);
    return;
}

void
generateCLTU_impl::crcEncoder(std::vector<unsigned char> &message, crc const initial_remainder)
{
    uint8_t data;
    crc remainder = initial_remainder;

    /*
         * Divide the message by the polynomial, a byte at a time.
         */
    for (int byte = 0; byte < message.size(); ++byte)
    {
        data = message.at(byte) ^ (remainder >> (WIDTH - 8));
        //remainder = crcTable[data] ^ (remainder << 8);
        //remainder = crcTable[data] ^ (remainder << 8);
        remainder = crcTable[message.at(byte) ^ remainder];
    }

    //Flip the remainder
    remainder ^= 0xFF;
    remainder <<= 1;
    //Set filler bit to 0 (and with 1111 1110)
    remainder &= 0xFE;

    /*
         * The final remainder is the CRC.
         */
    message.push_back(remainder);
    //return message;
}


void
generateCLTU_impl::bchEncoder(std::vector<unsigned char> &message)
{
    std::vector<unsigned char> information; // 7 bytes
    std::vector<unsigned char> codewords; // Encoded message

    //Ensure integral information symbols
    while(!(message.size() % BCH_INFORMATION_BLOCK) == 0)
      {
        message.push_back(0x55);
      }

    for(int i = 0; i < message.size(); i += BCH_INFORMATION_BLOCK)
    {
        //Copy 7 information bytes
        std::copy(message.begin() + i, message.begin() + i + BCH_INFORMATION_BLOCK,
                      std::back_inserter(information));
        //BCH encoding
        crcEncoder(information,INITIAL_REMAINDER);

        //Copy encoded information bits
        codewords.insert(codewords.end(), information.begin(), information.end());

        //Clear information bytes
        information.clear();
    }
    message = codewords;
}

void
generateCLTU_impl::crcInit()
{
    crc  remainder;


    /*
         * Compute the remainder of each possible dividend.
         */
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        /*
             * Start with the dividend followed by zeros.
             */
        remainder = dividend << (WIDTH - 8);

        /*
             * Perform modulo-2 division, a bit at a time.
             */
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            /*
                 * Try to divide the current data bit.
                 */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
             * Store the result into the table.
             */
        crcTable[dividend] = remainder;
        //std::cout << "Remainder from table : " << int (remainder&0xffff) << std::endl;
    }
}

int
generateCLTU_impl::work(int noutput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items)
{
    //const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
    //<+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace ccsds */
} /* namespace gr */

