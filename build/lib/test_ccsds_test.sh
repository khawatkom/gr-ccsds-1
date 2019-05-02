#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib
export PATH=/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/build/lib:$PATH
export LD_LIBRARY_PATH=/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-ccsds 
