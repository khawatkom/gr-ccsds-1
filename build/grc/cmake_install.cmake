# Install script for directory: /home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_convEncoder.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_convDecoder.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_generateCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_extractCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_processCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_synchronizeCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_generateCLTU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_genCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_simplifiedPLOP1.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_cc_depuncture.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_createCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_recoverCADU.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_encodeRS.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_decodeRS.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_encodeTurbo.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_synchronizeCADUSoft.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_recoverCADUSoft.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_decodeTurbo.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_synchronizeCADUHard.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_encodeLDPC.xml"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/grc/ccsds_decodeLDPC.xml"
    )
endif()

