#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Turbo Verification V4
# Generated: Tue Apr 23 15:27:10 2019
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
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import ccsds
import math, numpy, os
import sys
from gnuradio import qtgui


class turbo_verification_v4(gr.top_block, Qt.QWidget):

    def __init__(self, syncword="1ACFFC1D"):
        gr.top_block.__init__(self, "Turbo Verification V4")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Turbo Verification V4")
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

        self.settings = Qt.QSettings("GNU Radio", "turbo_verification_v4")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Parameters
        ##################################################
        self.syncword = syncword

        ##################################################
        # Variables
        ##################################################
        self.vcdu_size = vcdu_size = 223- 10
        self.value = value = [0,1]
        self.symbol = symbol = [-1,1]
        self.scramble = scramble = 0
        self.samp_rate = samp_rate = 2e6
        self.reset = reset = vcdu_size
        self.frame_size = frame_size = vcdu_size + 10
        self.Rc = Rc = 0.25

        ##################################################
        # Blocks
        ##################################################
        self.digital_map_bb_0 = digital.map_bb((-1,1))
        self.ccsds_synchronizeCADUSoft_0 = ccsds.synchronizeCADUSoft('1ACFFC1D',1,7,2,int(frame_size*8/Rc) + 2 + 4*8,0,0,'syncsoft')
        self.ccsds_recoverCADUSoft_0 = ccsds.recoverCADUSoft(int(frame_size*8/Rc) + 2, 0, 'syncsoft')
        self.ccsds_decodeTurbo_0 = ccsds.decodeTurbo(223, 1, 4, 1, 0.707, 0)
        self.blocks_unpack_k_bits_bb_2 = blocks.unpack_k_bits_bb(8)
        self.blocks_pdu_to_tagged_stream_0 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'packet_len')
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_char*1)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, '/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/Turbo/memory/build/turbo_frames.hex', True)
        self.blocks_char_to_float_0 = blocks.char_to_float(1, 1)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_decodeTurbo_0, 'out'), (self.blocks_pdu_to_tagged_stream_0, 'pdus'))
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.ccsds_decodeTurbo_0, 'in'))
        self.connect((self.blocks_char_to_float_0, 0), (self.ccsds_synchronizeCADUSoft_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_unpack_k_bits_bb_2, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0, 0), (self.blocks_null_sink_1, 0))
        self.connect((self.blocks_unpack_k_bits_bb_2, 0), (self.digital_map_bb_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.ccsds_recoverCADUSoft_0, 0))
        self.connect((self.digital_map_bb_0, 0), (self.blocks_char_to_float_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "turbo_verification_v4")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_syncword(self):
        return self.syncword

    def set_syncword(self, syncword):
        self.syncword = syncword

    def get_vcdu_size(self):
        return self.vcdu_size

    def set_vcdu_size(self, vcdu_size):
        self.vcdu_size = vcdu_size
        self.set_frame_size(self.vcdu_size + 10)
        self.set_reset(self.vcdu_size)

    def get_value(self):
        return self.value

    def set_value(self, value):
        self.value = value

    def get_symbol(self):
        return self.symbol

    def set_symbol(self, symbol):
        self.symbol = symbol

    def get_scramble(self):
        return self.scramble

    def set_scramble(self, scramble):
        self.scramble = scramble

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_reset(self):
        return self.reset

    def set_reset(self, reset):
        self.reset = reset

    def get_frame_size(self):
        return self.frame_size

    def set_frame_size(self, frame_size):
        self.frame_size = frame_size

    def get_Rc(self):
        return self.Rc

    def set_Rc(self, Rc):
        self.Rc = Rc


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    parser.add_option(
        "", "--syncword", dest="syncword", type="string", default="1ACFFC1D",
        help="Set sync [default=%default]")
    return parser


def main(top_block_cls=turbo_verification_v4, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls(syncword=options.syncword)
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
