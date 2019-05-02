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
#include "simplifiedPLOP1_impl.h"

namespace gr {
  namespace ccsds {

    simplifiedPLOP1::sptr
    simplifiedPLOP1::make(int idleSeqLen, int acqSeqLen)
    {
      return gnuradio::get_initial_sptr
        (new simplifiedPLOP1_impl(idleSeqLen, acqSeqLen));
    }

    /*
     * The private constructor
     */
    simplifiedPLOP1_impl::simplifiedPLOP1_impl(int idleSeqLen, int acqSeqLen)
      : gr::sync_block("simplifiedPLOP1",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_idleSeqLen(idleSeqLen),d_acqSeqLen(acqSeqLen)
    {

        message_port_register_in(pmt::mp("in"));
        message_port_register_out(pmt::mp("plop"));
        set_msg_handler(
                        pmt::mp("in"),
                        boost::bind(&simplifiedPLOP1_impl::plop1, this, _1)
                        );

        //Initializing idle and acquision sequences
        idleSeq = (uint8_t*)malloc(d_idleSeqLen*sizeof(uint8_t));
        acqSeq  = (uint8_t*)malloc(d_acqSeqLen*sizeof(uint8_t));
        memset(idleSeq,0xaa,d_idleSeqLen);
        memset(acqSeq,0x55,d_acqSeqLen);

    }

    /*
     * Our virtual destructor.
     */
    simplifiedPLOP1_impl::~simplifiedPLOP1_impl()
    {
    }

    void
    simplifiedPLOP1_impl::plop1(pmt::pmt_t pmt_msg)
    {
        //Extracting message from pmt
        pmt::pmt_t msg = pmt::cdr(pmt_msg);
        size_t offset(0);
        uint8_t *cltu = (uint8_t*)malloc(pmt::length(msg));
        memcpy(cltu,pmt::uniform_vector_elements(msg, offset),pmt::length(msg));

        //Creating a plopped packet
        uint8_t *plopPkt = (uint8_t*)malloc(pmt::length(msg) + d_acqSeqLen + 2*d_idleSeqLen);

        //Adding acquisition and idle sequences
        memcpy(plopPkt,  acqSeq,d_acqSeqLen);
        memcpy(plopPkt + (d_acqSeqLen), idleSeq, d_idleSeqLen);
        memcpy(plopPkt + (d_acqSeqLen + d_idleSeqLen),cltu,pmt::length(msg));
        memcpy(plopPkt + (d_acqSeqLen + d_idleSeqLen + pmt::length(msg)), idleSeq,d_idleSeqLen);

        //Publishing packet
        //Publish TC message
        pmt::pmt_t plop_pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(plopPkt,
                                                                  (pmt::length(msg) + d_acqSeqLen + 2*d_idleSeqLen))));
        message_port_pub(pmt::mp("plop"), plop_pdu);

    }

    int
    simplifiedPLOP1_impl::work(int noutput_items,
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

