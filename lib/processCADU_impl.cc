/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * Author   : Moses Browne Mwakyanjala
 * Date     : July 24th, 2018
 * Institue : Lulea University of Technology
 * E-mail   : moses.browne.mwakyanjala@ltu.se
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
#include "processCADU_impl.h"
//#include "fec/ReedSolomon/ReedSolomon.h"
#include "fec/Scrambler/Scrambler.h"
#include "fec/rs/reedsolomon.h"
//#include "fec/rs/rs"
#include <stdlib.h>

namespace gr {
namespace ccsds {

processCADU::sptr
processCADU::make(int frameLength, int scramble, int rs, int intDepth, std::string tagName)
{
    return gnuradio::get_initial_sptr
            (new processCADU_impl(frameLength, scramble, rs, intDepth, tagName));
}

/*
     * The private constructor
     */
processCADU_impl::processCADU_impl(int frameLength, int scramble, int rs, int intDepth, std::string tagName)
    : gr::sync_block("processCADU",
                     gr::io_signature::make(1, 1, sizeof(unsigned char)),
                     gr::io_signature::make(0, 0, 0)),
      d_frameLength(frameLength),d_scramble(scramble == 1),d_rs(rs), d_basis(rs >= 2), d_intDepth(intDepth)
{
    //Registering output port
    message_port_register_out(pmt::mp("out"));

    //Initiliazing RS decoders
    switch(d_rs)
    {
    case 0:
        //No code. Nothing to do
        d_basis = 0;
        d_type  = 0;
        break;
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

    //Scrambler object
    S = new Scrambler();

    if (d_rs) d_frameLength = d_frameLength +  32 * d_intDepth;

    //Multiple input
    set_output_multiple(d_frameLength * 8);


    //SEtting tag name
    key = pmt::mp(tagName);
    d_pack = new blocks::kernel::pack_k_bits(8);
}

/*
     * Our virtual destructor.
     */
processCADU_impl::~processCADU_impl()
{
    delete d_pack;
    delete S;
}

int
processCADU_impl::work(int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
{
    const unsigned char *in = (const unsigned char *) input_items[0];

    unsigned char frame_data[d_frameLength];

    d_tags.clear();
    int wer;
    reedSolomon rs(d_type,d_intDepth,d_basis);

    this->get_tags_in_window(d_tags, 0, 0, noutput_items,key);
    for(d_tags_itr = d_tags.begin(); d_tags_itr != d_tags.end(); d_tags_itr++) {
        // Check that we have enough data for a full frame
        if ((d_tags_itr->offset - this->nitems_read(0)) > (noutput_items - (d_frameLength) * 8))
        {
            return (d_tags_itr->offset - this->nitems_read(0) - 1);
        }
        //Pack bits into bytes
        d_pack->pack(frame_data, &in[d_tags_itr->offset - this->nitems_read(0)], d_frameLength);

        //Copying frame into std::vector buffer
        frameBuffer.insert(frameBuffer.begin(),frame_data, frame_data + d_frameLength);

        //Optional scrambling
        if (d_scramble) S->Scramble(frameBuffer);

        //Optional Reed-Solomon
        int errors;
        if(d_rs)
        {
            errors = rs.decode(frameBuffer,wer);


            if (errors >= 0)
            {
                //std::cout << "Success" << std::endl;
                pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(frameBuffer.data(),frameBuffer.size())));
                message_port_pub(pmt::mp("out"), pdu);
            }
            else // Failure
            {
                //std::cout << std::endl << "RS failure" << std::endl;
                pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(frameBuffer.data(),frameBuffer.size())));
                message_port_pub(pmt::mp("out"), pdu);
            }
        }
        else{
            pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(frameBuffer.data(),frameBuffer.size())));
            message_port_pub(pmt::mp("out"), pdu);
        }

        //Clear buffers
        frameBuffer.clear();
        //errors.clear();
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace ccsds */
} /* namespace gr */

