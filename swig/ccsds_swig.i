/* -*- c++ -*- */

#define CCSDS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ccsds_swig_doc.i"

%{
#include "ccsds/convEncoder.h"
#include "ccsds/convDecoder.h"
#include "ccsds/generateCADU.h"
#include "ccsds/processCADU.h"
#include "ccsds/extractCADU.h"
#include "ccsds/synchronizeCADU.h"
#include "ccsds/generateCLTU.h"
#include "ccsds/genCADU.h"
#include "ccsds/simplifiedPLOP1.h"
#include "ccsds/cc_depuncture.h"
#include "ccsds/createCADU.h"
#include "ccsds/recoverCADU.h"
#include "ccsds/encodeRS.h"
#include "ccsds/decodeRS.h"
#include "ccsds/encodeTurbo.h"
#include "ccsds/synchronizeCADUSoft.h"
#include "ccsds/recoverCADUSoft.h"
#include "ccsds/decodeTurbo.h"
#include "ccsds/synchronizeCADUHard.h"
#include "ccsds/encodeLDPC.h"
#include "ccsds/decodeLDPC.h"
%}


%include "ccsds/convEncoder.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, convEncoder);
%include "ccsds/convDecoder.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, convDecoder);

%include "ccsds/generateCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, generateCADU);

%include "ccsds/processCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, processCADU);
%include "ccsds/extractCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, extractCADU);

%include "ccsds/synchronizeCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, synchronizeCADU);
%include "ccsds/generateCLTU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, generateCLTU);
%include "ccsds/genCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, genCADU);
%include "ccsds/simplifiedPLOP1.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, simplifiedPLOP1);
%include "ccsds/cc_depuncture.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, cc_depuncture);
%include "ccsds/createCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, createCADU);
%include "ccsds/recoverCADU.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, recoverCADU);
%include "ccsds/encodeRS.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, encodeRS);
%include "ccsds/decodeRS.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, decodeRS);

%include "ccsds/encodeTurbo.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, encodeTurbo);

%include "ccsds/synchronizeCADUSoft.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, synchronizeCADUSoft);
%include "ccsds/recoverCADUSoft.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, recoverCADUSoft);
%include "ccsds/decodeTurbo.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, decodeTurbo);
%include "ccsds/synchronizeCADUHard.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, synchronizeCADUHard);
%include "ccsds/encodeLDPC.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, encodeLDPC);
%include "ccsds/decodeLDPC.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, decodeLDPC);
