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
install(FILES
    ccsds_convEncoder.xml
    ccsds_convDecoder.xml
    ccsds_generateCADU.xml
    ccsds_extractCADU.xml
    ccsds_processCADU.xml
    ccsds_synchronizeCADU.xml
    ccsds_generateCLTU.xml
    ccsds_genCADU.xml
    ccsds_simplifiedPLOP1.xml
    ccsds_cc_depuncture.xml
    ccsds_createCADU.xml
    ccsds_recoverCADU.xml
    ccsds_encodeRS.xml
    ccsds_decodeRS.xml
    ccsds_encodeTurbo.xml
    ccsds_synchronizeCADUSoft.xml
    ccsds_recoverCADUSoft.xml
    ccsds_decodeTurbo.xml
    ccsds_synchronizeCADUHard.xml
    ccsds_encodeLDPC.xml
    ccsds_decodeLDPC.xml DESTINATION share/gnuradio/grc/blocks
)
