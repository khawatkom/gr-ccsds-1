# Install script for directory: /home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ccsds" TYPE FILE FILES
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/api.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/convEncoder.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/convDecoder.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/generateCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/processCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/extractCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/synchronizeCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/generateCLTU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/genCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/simplifiedPLOP1.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/cc_depuncture.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/createCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/recoverCADU.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/encodeRS.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/decodeRS.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/encodeTurbo.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/synchronizeCADUSoft.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/recoverCADUSoft.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/decodeTurbo.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/synchronizeCADUHard.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/encodeLDPC.h"
    "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/include/ccsds/decodeLDPC.h"
    )
endif()

