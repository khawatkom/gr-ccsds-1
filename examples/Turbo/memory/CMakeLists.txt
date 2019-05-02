# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
cmake_minimum_required(VERSION 3.5)
include_directories(
	${GR_BLOCKS_INCLUDE_DIRS}
	${GR_DIGITAL_INCLUDE_DIRS}
        ${GR_PMT_INCLUDE_DIRS}
	${GNURADIO_RUNTIME_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
)
set (CMAKE_C_FLAGS "-Wall -g")
add_executable(turbo_memory turbo_memory.cc)
find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(turbo_memory
  gnuradio-runtime gnuradio-blocks gnuradio-digital gnuradio-ccsds gnuradio-tdd gnuradio-pmt ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

#INSTALL(TARGETS
#    turbo_memory
#    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
#)
