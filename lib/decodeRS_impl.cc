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
#include "decodeRS_impl.h"

namespace gr {
  namespace ccsds {

    decodeRS::sptr
    decodeRS::make(int code, int intDepth, int pass_all, int fert, int wert)
    {
      return gnuradio::get_initial_sptr
        (new decodeRS_impl(code, intDepth,pass_all,fert,wert));
    }

    /*
     * The private constructor
     */
    decodeRS_impl::decodeRS_impl(int code, int intDepth, int pass_all, int fert, int wert)
      : gr::sync_block("decodeRS",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_intDepth(intDepth), d_pass_all(pass_all > 0),d_fert(fert > 0), d_rx_cnt(0),d_valid_cnt(0),
        d_wert(wert > 0),d_rx_wer_cnt(0),d_wrong_wer_cnt(0)
    {
        message_port_register_in(pmt::mp("cadu"));
        message_port_register_out(pmt::mp("vcdu"));
        set_msg_handler(
                                pmt::mp("cadu"),
                                boost::bind(&decodeRS_impl::rsdecode, this, _1)
                                );

        //Initiliazing RS decoders
        switch(code)
        {
        case 1: //RS(255,223) + Conv
            d_basis = 0;
            d_type  = 0;
            break;
        case 2: //RS(255,223) + Dual basis
            d_basis = 1;
            d_type  = 0;
            break;
        case 3: //RS(255,239) + Conv
            d_basis = 0;
            d_type  = 1;
            break;
        case 4: //RS(255,239) + Dual basis
            d_basis = 1;
            d_type  = 1;
            break;
        default:
            d_basis = 0;
            d_type  = 0;
            //Nothing to do
        }
    }

    /*
     * Our virtual destructor.
     */
    decodeRS_impl::~decodeRS_impl()
    {
    }

    bool
    decodeRS_impl::crc_ccitt(std::vector<unsigned char> frame)
    {

        unsigned short crc = frame.at(frame.size() - 1) << 8
                           | frame.at(frame.size() - 2);

        unsigned int POLY=0x8408; //reflected 0x1021
        unsigned short calc_crc=0xFFFF;
        for(size_t i=0; i<frame.size() - 2; i++) {
            calc_crc ^= frame.at(i);
            for(size_t j=0; j<8; j++) {
                if(calc_crc&0x01) calc_crc = (calc_crc >> 1) ^ POLY;
                else         calc_crc = (calc_crc >> 1);
            }
        }
        calc_crc ^= 0xFFFF;

        return (calc_crc == crc);
    }

    void
    decodeRS_impl::rsdecode(pmt::pmt_t pmt_msg)
    {
        //Extracting cadu from pmt
        pmt::pmt_t msg = pmt::cdr(pmt_msg);
        size_t offset(0);
        uint8_t *cadu = (uint8_t*)malloc(pmt::length(msg));
        memcpy(cadu,pmt::uniform_vector_elements(msg, offset),pmt::length(msg));
        std::vector<uint8_t> cadu_buffer;
        cadu_buffer.insert(cadu_buffer.end(), cadu, cadu +  pmt::length(msg));
        free(cadu);

        //Reed_Solomon decoding
        int wer;
        reedSolomon rs(d_type,d_intDepth,d_basis);
        int errors = rs.decode(cadu_buffer,d_wer);
        std::cout.precision(4);

        //Frame error rate calculation and reporting
        if (d_fert)
        {
            //Increase the number of received codeblocks
            d_rx_cnt ++;

            //Increase the number of valid codeblocks
            /*if (crc_ccitt(cadu_buffer))
            {
                d_valid_cnt ++;
            }*/
            if (errors >= 0)
            {
                d_valid_cnt ++;
            }
            //Printing FERT information
            std::cout << " Total codeblocks  : "
                      << d_rx_cnt
                      << " Valid codeblocks   : "
                      << d_valid_cnt
                      << " FER  =  "
                      << std::scientific
                      << float((float(d_rx_cnt) - float(d_valid_cnt))/float(d_rx_cnt))
                      << std::endl;
        }
        //std::cout << "Number of errors : " << errors << std::endl;

        //Word error rate calculation and reporting
        if (d_wert)
        {
            //Increase the number of received words
            d_rx_wer_cnt += d_intDepth;
            d_wrong_wer_cnt += d_wer;
            //std::cout << "Local WER : " << wer;
            //Printing WERT information
            std::cout << " Total words  : "
                      << d_rx_wer_cnt
                      << " Wrong words   : "
                      << d_wrong_wer_cnt
                      << " WER  =  "
                      << std::scientific
                      << float(d_wrong_wer_cnt)/float(d_rx_wer_cnt)
                      << std::endl;

        }
        //Publish a packet
        if ((errors >= 0) || d_pass_all)
        {
            pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(cadu_buffer.data(),cadu_buffer.size())));
            message_port_pub(pmt::mp("vcdu"), pdu);
        }
        //std::cout << "CADU BUFFER SIZE: " << cadu_buffer.size() << std::endl;

    }

    int
    decodeRS_impl::work(int noutput_items,
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

