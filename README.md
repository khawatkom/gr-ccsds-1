# CCSDS FEC Codes Stack
This GNU Radio OOT implements CCSDS FEC codes as specified by the CCSDS TM Synchonization and Channel Coding 
(CCSDS 131.0-B-2, August 2011).

## FEC Codes Supported
- CC(7,1/2)
- RS(255,223) and RS(255,239)
- LDPC(8176,8154) and LDPC(8160,7136) [aka C2]
- Turbo Codes [based on Gianluca's deepspace-turbo module and gr-dslwp]
- Frame sync, with automatic ambiguity resolution

## Dependencies
The flowgraphs use the gr-mapper OOT to evaluate BER performance.

git clone https://github.com/gr-vt/gr-mapper.git

mkdir build && cd build

cmake .. && make

sudo make install && sudo ldconfig

## Installation
git clone https://github.com/mbkitine/gr-ccsds.git

mkdir build && cd build

cmake .. && make

sudo make install && sudo ldconfig

## Usage
See examples folder. 
Configuration of LDPC Encoder and Decoder
- The LDPC encoder uses circulants to create the generator matrix. The circulant file is under /lib/fec/ldpc/gmini/C2.txt
- The LDPC decoder uses alist file to create the parity check matrix. The alist file is under /lib/fec/ldpc/alist/C2_alist.a

## Issues
- Memory leaks in the LDPC Decoder (See examples/LDPC/ldpc_bit_error_rate)
- Memory leaks in the Turbo Decoder (See examples/Turbo/turbo_bit_error_rate)
