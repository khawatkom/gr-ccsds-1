#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Thu Feb 21 15:01:00 2019
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import ccsds
import mapper
import math, numpy
import sys
import tdd
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.vcdu_size = vcdu_size = 100
        self.symbol_rate = symbol_rate = 500e3
        self.sps = sps = 4
        self.samp_rate = samp_rate = symbol_rate*sps
        self.ntaps = ntaps = 40*sps
        self.nfilts = nfilts = 32
        self.frame_size = frame_size = vcdu_size + 10
        self.value = value = [0,1]
        self.to = to = 0.0

        self.taps = taps = firdes.root_raised_cosine(sps, samp_rate, symbol_rate, 0.35, ntaps)

        self.symbol = symbol = [-1,1]
        self.scramble = scramble = 1
        self.rs = rs = 1
        self.reset = reset = frame_size
        self.polyphase_taps = polyphase_taps = firdes.root_raised_cosine(sps*nfilts, nfilts, 1.0/float(sps), 0.35, 20*sps*nfilts)
        self.parity = parity = 32
        self.linecode = linecode = 1
        self.intDepth = intDepth = 1
        self.cfo = cfo = 0.25
        self.SNR = SNR = 10

        ##################################################
        # Blocks
        ##################################################
        self.tdd_ferMsgGen_0 = tdd.ferMsgGen(vcdu_size, 'cadu_len', 'vcdu_len')
        self.tdd_ferMsgCount_0 = tdd.ferMsgCount(1)
        self.mapper_prbs_source_b_0 = mapper.prbs_source_b("PRBS31", reset)
        self.ccsds_synchronizeCADU_0 = ccsds.synchronizeCADU('1ACFFC1D', 0, "sync")
        self.ccsds_recoverCADU_1 = ccsds.recoverCADU(frame_size  + intDepth*parity, scramble, 'sync')
        self.ccsds_encodeRS_0 = ccsds.encodeRS(rs, frame_size, intDepth, 'rshh', 'cadu_len')
        self.ccsds_decodeRS_0 = ccsds.decodeRS(rs, intDepth,0,0)
        self.ccsds_createCADU_0 = ccsds.createCADU(frame_size + intDepth*parity, '1ACFFC1D', scramble, 'rshh')
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, vcdu_size, "vcdu_len")
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_message_debug_0 = blocks.message_debug()

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_decodeRS_0, 'vcdu'), (self.tdd_ferMsgCount_0, 'in'))
        self.msg_connect((self.ccsds_recoverCADU_1, 'cadu'), (self.ccsds_decodeRS_0, 'cadu'))
        self.msg_connect((self.tdd_ferMsgCount_0, 'out'), (self.blocks_message_debug_0, 'store'))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.tdd_ferMsgGen_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.ccsds_synchronizeCADU_0, 0))
        self.connect((self.ccsds_createCADU_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.ccsds_encodeRS_0, 0), (self.ccsds_createCADU_0, 0))
        self.connect((self.ccsds_synchronizeCADU_0, 0), (self.ccsds_recoverCADU_1, 0))
        self.connect((self.mapper_prbs_source_b_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.tdd_ferMsgGen_0, 0), (self.ccsds_encodeRS_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_vcdu_size(self):
        return self.vcdu_size

    def set_vcdu_size(self, vcdu_size):
        self.vcdu_size = vcdu_size
        self.set_frame_size(self.vcdu_size + 10)
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.vcdu_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.vcdu_size)

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_samp_rate(self.symbol_rate*self.sps)

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.set_samp_rate(self.symbol_rate*self.sps)
        self.set_polyphase_taps(firdes.root_raised_cosine(self.sps*self.nfilts, self.nfilts, 1.0/float(self.sps), 0.35, 20*self.sps*self.nfilts))
        self.set_ntaps(40*self.sps)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_ntaps(self):
        return self.ntaps

    def set_ntaps(self, ntaps):
        self.ntaps = ntaps

    def get_nfilts(self):
        return self.nfilts

    def set_nfilts(self, nfilts):
        self.nfilts = nfilts
        self.set_polyphase_taps(firdes.root_raised_cosine(self.sps*self.nfilts, self.nfilts, 1.0/float(self.sps), 0.35, 20*self.sps*self.nfilts))

    def get_frame_size(self):
        return self.frame_size

    def set_frame_size(self, frame_size):
        self.frame_size = frame_size
        self.set_reset(self.frame_size)

    def get_value(self):
        return self.value

    def set_value(self, value):
        self.value = value

    def get_to(self):
        return self.to

    def set_to(self, to):
        self.to = to

    def get_taps(self):
        return self.taps

    def set_taps(self, taps):
        self.taps = taps

    def get_symbol(self):
        return self.symbol

    def set_symbol(self, symbol):
        self.symbol = symbol

    def get_scramble(self):
        return self.scramble

    def set_scramble(self, scramble):
        self.scramble = scramble

    def get_rs(self):
        return self.rs

    def set_rs(self, rs):
        self.rs = rs

    def get_reset(self):
        return self.reset

    def set_reset(self, reset):
        self.reset = reset

    def get_polyphase_taps(self):
        return self.polyphase_taps

    def set_polyphase_taps(self, polyphase_taps):
        self.polyphase_taps = polyphase_taps

    def get_parity(self):
        return self.parity

    def set_parity(self, parity):
        self.parity = parity

    def get_linecode(self):
        return self.linecode

    def set_linecode(self, linecode):
        self.linecode = linecode

    def get_intDepth(self):
        return self.intDepth

    def set_intDepth(self, intDepth):
        self.intDepth = intDepth

    def get_cfo(self):
        return self.cfo

    def set_cfo(self, cfo):
        self.cfo = cfo

    def get_SNR(self):
        return self.SNR

    def set_SNR(self, SNR):
        self.SNR = SNR


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
