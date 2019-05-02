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
#include "cc_depuncture_impl.h"
#include <volk/volk.h>
#include <boost/bind.hpp>
#include <pmt/pmt.h>
#include <string>
namespace gr {
namespace ccsds {

cc_depuncture::sptr
cc_depuncture::make(int puncsize, int puncpat, int delay, char symbol)
{
    return gnuradio::get_initial_sptr
            (new cc_depuncture_impl(puncsize, puncpat, delay, symbol));
}

/*
     * The private constructor
     */
cc_depuncture_impl::cc_depuncture_impl(int puncsize, int puncpat, int delay, char symbol)
    : gr::block("cc_depuncture",
                gr::io_signature::make(1, 1, sizeof(float)),
                gr::io_signature::make(1, 1, sizeof(float))),
      d_puncsize(puncsize), d_delay(delay), d_sym(symbol)
{
    // Create a mask of all 1's of puncsize length
    int mask = 0;
    for(int i = 0; i < d_puncsize; i++)
        mask |= 1 << i;

    // Rotate the pattern for the delay value; then mask it if there
    // are any excess 1's in the pattern.
    for(int i = 0; i < d_delay; ++i) {
        puncpat = ((puncpat & 1) << (d_puncsize - 1)) + (puncpat >> 1);
    }
    d_puncpat = puncpat & mask;

    // Calculate the number of holes in the pattern. The mask is all
    // 1's given puncsize and puncpat is a pattern with >= puncsize
    // 0's (masked to ensure this). The difference between the
    // number of 1's in the mask and the puncpat is the number of
    // holes.
    uint32_t count_mask=0, count_pat=0;
    volk_32u_popcnt(&count_mask, static_cast<uint32_t>(mask));
    volk_32u_popcnt(&count_pat, static_cast<uint32_t>(d_puncpat));
    d_puncholes = count_mask - count_pat;

    set_fixed_rate(true);
    set_relative_rate((double)d_puncsize/(d_puncsize - d_puncholes));
    set_output_multiple(d_puncsize);
    //set_msg_handler(boost::bind(&depuncture_bb_impl::catch_msg, this, _1));
}

/*
     * Our virtual destructor.
     */
cc_depuncture_impl::~cc_depuncture_impl()
{
}

int
cc_depuncture_impl::fixed_rate_ninput_to_noutput(int ninput)
{
    return (int)(((d_puncsize/(double)(d_puncsize - d_puncholes)) * ninput) + .5);
}

int
cc_depuncture_impl::fixed_rate_noutput_to_ninput(int noutput)
{
    return (int)((((d_puncsize - d_puncholes)/(double)(d_puncsize)) * noutput) + .5);
}

void
cc_depuncture_impl::forecast(int noutput_items,
                             gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = (int)((((d_puncsize - d_puncholes)/(double)(d_puncsize)) * noutput_items) + .5);
}

int
cc_depuncture_impl::general_work (int noutput_items,
                                  gr_vector_int &ninput_items,
                                  gr_vector_const_void_star &input_items,
                                  gr_vector_void_star &output_items)
{
    const float *in = (const float*)input_items[0];
    float *out = (float*)output_items[0];

    for(int i=0, k=0; i < noutput_items/output_multiple(); ++i) {
        for(int j = 0; j < output_multiple(); ++j) {
            out[i*output_multiple() + j] = ((d_puncpat >> (d_puncsize - 1 - j)) & 1) ? in[k++] : 0.0;
        }
    }

    consume_each((int)(((1.0/relative_rate()) * noutput_items) + .5));
    return noutput_items;
}

} /* namespace ccsds */
} /* namespace gr */

