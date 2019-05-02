#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri Feb 22 16:23:33 2019
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
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import fec
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import baseband
import ccsds
import os
import sys
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
        self.sync2 = sync2 = "0000001101000111011101101100011100100111001010001001010110110000"
        self.sync1 = sync1 = "00011010110011111111110000011101"
        self.sps = sps = 8
        self.samp_rate = samp_rate = 32000


        self.encoder = encoder = fec.cc_encoder_make(1293, 7, 2, ([79,109]), 0, fec.CC_STREAMING, False)



        self.decoder = decoder = fec.cc_decoder.make(1293, 7, 2, ([79,109]), 0, -1, fec.CC_STREAMING, False)


        ##################################################
        # Blocks
        ##################################################
        self.fec_puncture_xx_0 = fec.puncture_bb(4, 13, 0)
        self.digital_map_bb_0 = digital.map_bb(([-1,1]))
        self.digital_correlate_access_code_tag_bb_0 = digital.correlate_access_code_tag_bb(sync1, 0, "sync")
        self.ccsds_convEncoder_0 = ccsds.convEncoder(1)
        self.ccsds_convDecoder_0 = ccsds.convDecoder(1273, ([79,-109]), 0, 0)
        self.ccsds_cc_depuncture_0 = ccsds.cc_depuncture(4, 13, 0, 0)
        self.blocks_unpack_k_bits_bb_1 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_gate_0 = blocks.tag_gate(gr.sizeof_char * 1, False)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_char*1, '', ''); self.blocks_tag_debug_0.set_display(True)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, '/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/ConvolutionalCode/conv.bin', False)
        self.blocks_char_to_float_1 = blocks.char_to_float(1, 1)
        self.baseband_debug_ccsds_deframer_0 = baseband.debug_ccsds_deframer(1, 113)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.baseband_debug_ccsds_deframer_0, 'out'), (self.blocks_message_debug_0, 'print_pdu'))
        self.connect((self.blocks_char_to_float_1, 0), (self.ccsds_cc_depuncture_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_tag_gate_0, 0), (self.blocks_tag_debug_0, 0))
        self.connect((self.blocks_tag_gate_0, 0), (self.digital_correlate_access_code_tag_bb_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_unpack_k_bits_bb_1, 0))
        self.connect((self.blocks_unpack_k_bits_bb_1, 0), (self.ccsds_convEncoder_0, 0))
        self.connect((self.ccsds_cc_depuncture_0, 0), (self.ccsds_convDecoder_0, 0))
        self.connect((self.ccsds_convDecoder_0, 0), (self.blocks_tag_gate_0, 0))
        self.connect((self.ccsds_convEncoder_0, 0), (self.fec_puncture_xx_0, 0))
        self.connect((self.digital_correlate_access_code_tag_bb_0, 0), (self.baseband_debug_ccsds_deframer_0, 0))
        self.connect((self.digital_map_bb_0, 0), (self.blocks_char_to_float_1, 0))
        self.connect((self.fec_puncture_xx_0, 0), (self.digital_map_bb_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def setStyleSheetFromFile(self, filename):
        try:
            if not os.path.exists(filename):
                filename = os.path.join(
                    gr.prefix(), "share", "gnuradio", "themes", filename)
            with open(filename) as ss:
                self.setStyleSheet(ss.read())
        except Exception as e:
            print >> sys.stderr, e

    def get_sync2(self):
        return self.sync2

    def set_sync2(self, sync2):
        self.sync2 = sync2

    def get_sync1(self):
        return self.sync1

    def set_sync1(self, sync1):
        self.sync1 = sync1

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_encoder(self):
        return self.encoder

    def set_encoder(self, encoder):
        self.encoder = encoder

    def get_decoder(self):
        return self.decoder

    def set_decoder(self, decoder):
        self.decoder = decoder


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.setStyleSheetFromFile('/home/mbkitine/Pictures/Selection_017.png')
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
