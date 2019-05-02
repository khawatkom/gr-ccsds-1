/*
 * Copyright 2011,2013 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


/*
 * GNU Radio C++ example creating dial tone
 * ("the simplest thing that could possibly work")
 *
 * Send a tone each to the left and right channels of stereo audio
 * output and let the user's brain sum them.
 *
 * GNU Radio makes extensive use of Boost shared pointers.  Signal processing
 * blocks are typically created by calling a "make" factory function, which
 * returns an instance of the block as a typedef'd shared pointer that can
 * be used in any way a regular pointer can.  Shared pointers created this way
 * keep track of their memory and free it at the right time, so the user
 * doesn't need to worry about it (really).
 *
 */

// Include header files for each block used in flowgraph
#include <gnuradio/top_block.h>
//#include <gnuradio/analog/sig_source_f.h>
//#include <gnuradio/audio/sink.h>

#include <gnuradio/blocks/file_source.h>
#include <gnuradio/blocks/unpack_k_bits_bb.h>
#include <gnuradio/digital/map_bb.h>
#include <gnuradio/blocks/char_to_float.h>
#include <ccsds/synchronizeCADUSoft.h>
#include <ccsds/recoverCADUSoft.h>
#include <ccsds/decodeTurbo.h>
#include <tdd/nullMsgSink.h>
#include <cstdlib>

using namespace gr;
using namespace std;
int main(int argc, char **argv)
{
  int stop = atoi(argv[1]);
  std::cout << "Program starting .... " << std::endl;
  std::cout << "Number of packets : " << stop << std::endl;
  top_block_sptr tb = make_top_block("turbo_memory");

  //File source
  blocks::file_source::sptr file = blocks::file_source::make(sizeof(unsigned char),"turbo_frames.hex",true);

  //Unpacking bits
  blocks::unpack_k_bits_bb::sptr  unpack = gr::blocks::unpack_k_bits_bb::make(8);


  //Initializing the map block
  std::vector<int> map;
  map.push_back(-1);
  map.push_back(1);
  gr::digital::map_bb::sptr mapper(gr::digital::map_bb::make(map));

  //Char to float
  gr::blocks::char_to_float::sptr char_to_float(gr::blocks::char_to_float::make());

  //Synchronize cadu
  gr::ccsds::synchronizeCADUSoft::sptr sync = gr::ccsds::synchronizeCADUSoft::make("1ACFFC1D", 0, "sync");

  //Extract cadu
  gr::ccsds::recoverCADUSoft::sptr cadu = gr::ccsds::recoverCADUSoft::make(894,0,"sync");

  //Decode turbo

  gr::ccsds::decodeTurbo::sptr turbo = gr::ccsds::decodeTurbo::make(223, 1, 4, stop, 0.707, 0);

  //Null sink
  gr::tdd::nullMsgSink::sptr null = gr::tdd::nullMsgSink::make(1);


  tb->connect(file, 0, unpack,0);
  tb->connect(unpack, 0, mapper , 0);
  tb->connect(mapper,0,char_to_float,0);
  tb->connect(char_to_float,0,sync,0);
  tb->connect(sync,0,cadu,0);
  tb->msg_connect(cadu,pmt::intern("cadu"),turbo,pmt::intern("in"));
  tb->msg_connect(turbo, pmt::intern("out"),null,pmt::intern("in"));




  // Tell GNU Radio runtime to start flowgraph threads; the foreground thread
  // will block until either flowgraph exits (this example doesn't) or the
  // application receives SIGINT (e.g., user hits CTRL-C).
  //
  // Real applications may use tb->start() which returns, allowing the foreground
  // thread to proceed, then later use tb->stop(), followed by tb->wait(), to cleanup
  // GNU Radio before exiting.
  tb->run();

  // Exit normally.
  return 0;
}
