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
#include "recoverCADUSoft_impl.h"

namespace gr {
  namespace ccsds {

    recoverCADUSoft::sptr
    recoverCADUSoft::make(int frame_length, int scramble, std::string tag_name)
    {
      return gnuradio::get_initial_sptr
        (new recoverCADUSoft_impl(frame_length, scramble, tag_name));
    }

    /*
     * The private constructor
     */
    recoverCADUSoft_impl::recoverCADUSoft_impl(int frame_length, int scramble, std::string tag_name)
        : gr::sync_block("recoverCADUSoft",
                         gr::io_signature::make(1, 1, sizeof(float)),
                         gr::io_signature::make(0, 0, 0)),
          d_frame_length(frame_length), d_scramble(scramble == 1), d_ambiguity(0)
    {
        //Registering output port
        message_port_register_out(pmt::mp("cadu"));

        set_output_multiple(d_frame_length);

        //Setting tag name
        key = pmt::mp(tag_name);
        //d_pack = new blocks::kernel::pack_k_bits(8);
        ccsdsDescrambler = new Scrambler();
    }

    /*
     * Our virtual destructor.
     */
    recoverCADUSoft_impl::~recoverCADUSoft_impl()
    {
    }

    int
    recoverCADUSoft_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];



        d_tags.clear();

        this->get_tags_in_window(d_tags, 0, 0, noutput_items,key);
        for(d_tags_itr = d_tags.begin(); d_tags_itr != d_tags.end(); d_tags_itr++) {
            // Check that we have enough data for a full frame
            if ((d_tags_itr->offset - this->nitems_read(0)) > (noutput_items - (d_frame_length)))
            {
                return (d_tags_itr->offset - this->nitems_read(0) - 1);
            }
            float frame_data[d_frame_length];

            memcpy(frame_data,&in[d_tags_itr->offset - this->nitems_read(0)], d_frame_length*sizeof(float));

            //Ambiguity resolution
            d_ambiguity = (int)pmt::to_double(d_tags_itr->value);
            ambResolution(d_ambiguity,frame_data,d_frame_length);


            //Optional scrambling
            if (d_scramble) ccsdsDescrambler->descramble_soft(frame_data,d_frame_length);

            //Publish extracted CADU
            pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::init_f32vector(d_frame_length, frame_data)));
            //pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL,pmt::init_f32vector(frameBuffer.size(), frameBuffer.data())));
            message_port_pub(pmt::mp("cadu"), pdu);
        }
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

    void
    recoverCADUSoft_impl::ambResolution(int ambiguity, float frame_data[],int frame_length)
    {
        switch(ambiguity)
        {
        case 0:
            //No flipping is needed.

            break;
        case 90:
            for(int i = 0; i < frame_length; i++)
            {
                //frame_data[i] = IQSWAP[frame_data[i]] ^ 0x55;
            }
            break;
        case 180:
            for(int i = 0; i < frame_length; i++)
                frame_data[i] *= -1.0;
            break;
        case 270:
            for(int i = 0; i < frame_length; i++)
            {
                //frame_data[i] = IQSWAP[frame_data[i]] ^ 0xAA;
            }
            break;
        }
    }

  } /* namespace ccsds */
} /* namespace gr */

