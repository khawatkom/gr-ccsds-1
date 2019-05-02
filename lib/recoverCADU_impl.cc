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
#include "recoverCADU_impl.h"

namespace gr {
  namespace ccsds {

    recoverCADU::sptr
    recoverCADU::make(int frame_length, int scramble, std::string tag_name)
    {
      return gnuradio::get_initial_sptr
        (new recoverCADU_impl(frame_length, scramble, tag_name));
    }

    /*
     * The private constructor
     */
    recoverCADU_impl::recoverCADU_impl(int frame_length, int scramble, std::string tag_name)
      : gr::sync_block("recoverCADU",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(0, 0, 0)),
        d_frame_length(frame_length), d_scramble(scramble == 1),d_ambiguity(0)

    {
        //Registering output port
        message_port_register_out(pmt::mp("cadu"));

        set_output_multiple(d_frame_length * 8);

        //Setting tag name
        key = pmt::mp(tag_name);
        d_pack = new blocks::kernel::pack_k_bits(8);
        S = new Scrambler();

        //Initialize Ambiguity resolution vector
        for(unsigned int i = 0; i < 256; i++)
        {
            IQSWAP[i] = swapIQ(i&0xFF);
        }
    }

    unsigned char
    recoverCADU_impl::swap(unsigned char oct)
    {
        switch(oct)
        {
        case 0x0:
            oct = 0x0;
            break;
        case 0x1:
            oct = 0x2;
            break;
        case 0x2:
            oct = 0x1;
            break;
        case 0x3:
            oct = 0x3;
            break;
        case 0x4:
            oct = 0x8;
            break;
        case 0x5:
            oct = 0xA;
            break;
        case 0x6:
            oct = 0x9;
            break;
        case 0x7:
            oct = 0xB;
            break;
        case 0x8:
            oct = 0x4;
            break;
        case 0x9:
            oct = 0x6;
            break;
        case 0xA:
            oct = 0x5;
            break;
        case 0xB:
            oct = 0x7;
            break;
        case 0xC:
            oct = 0xC;
            break;
        case 0xD:
            oct = 0xE;
            break;
        case 0xE:
            oct = 0xD;
            break;
        case 0xF:
            oct = 0xF;
            break;
        }
       return oct;
    }

    unsigned char
    recoverCADU_impl::swapIQ(unsigned char in)
    {
        unsigned  char oct1 = (in & 0x0F);
        unsigned  char oct0 = ((in >> 4) & 0x0F);
        unsigned  char out;

        /* std::cout << "Oct 0 : " << std::hex << oct0 << std::endl;
        std::cout << "Oct 1 : " << std::hex << oct1 << std::endl;

        std::cout << "Swapped Oct 0 : " << std::hex << swap(oct0) << std::endl;
        std::cout << "Swapped Oct 1 : " << std::hex << swap(oct1) << std::endl;*/
        out = (swap(oct0) << 4);
        out |= swap(oct1);
        return out;
    }

    void
    recoverCADU_impl::ambResolution(int ambiguity, unsigned char frame_data[],int frame_length)
    {
        switch(ambiguity)
        {
        case 0:
	    //No flipping is needed.

            break;
        case 90:
            for(int i = 0; i < frame_length; i++)
	    {
                frame_data[i] = IQSWAP[frame_data[i]] ^ 0x55;
	    }
            break;
        case 180:
            for(int i = 0; i < frame_length; i++)
                frame_data[i]^= 0xFF;
            break;
        case 270:
	    for(int i = 0; i < frame_length; i++)
	    {
                frame_data[i] = IQSWAP[frame_data[i]] ^ 0xAA;
	    }
            break;
        }
    }

    /*
     * Our virtual destructor.
     */
    recoverCADU_impl::~recoverCADU_impl()
    {
        delete d_pack;
        delete S;
    }

    int
    recoverCADU_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];

        unsigned char frame_data[d_frame_length];

        d_tags.clear();

        this->get_tags_in_window(d_tags, 0, 0, noutput_items,key);
        for(d_tags_itr = d_tags.begin(); d_tags_itr != d_tags.end(); d_tags_itr++) {
            // Check that we have enough data for a full frame
            if ((d_tags_itr->offset - this->nitems_read(0)) > (noutput_items - (d_frame_length) * 8))
            {
                return (d_tags_itr->offset - this->nitems_read(0) - 1);
            }
            //Pack bits into bytes
            d_pack->pack(frame_data, &in[d_tags_itr->offset - this->nitems_read(0)], d_frame_length);

            //Ambiguity resolution
            d_ambiguity = (int)pmt::to_double(d_tags_itr->value);
            ambResolution(d_ambiguity,frame_data,d_frame_length);
            //std::cout << "Ambiguity : " << d_ambiguity << std::endl;


            //Copying frame into std::vector buffer
            frameBuffer.insert(frameBuffer.begin(),frame_data, frame_data + d_frame_length);

            //Optional scrambling
            if (d_scramble) S->Scramble(frameBuffer);

            //Public extracted CADU
            pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::make_blob(frameBuffer.data(),frameBuffer.size())));
            message_port_pub(pmt::mp("cadu"), pdu);

            //Clear buffers
            frameBuffer.clear();
            //errors.clear();
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace ccsds */
} /* namespace gr */

