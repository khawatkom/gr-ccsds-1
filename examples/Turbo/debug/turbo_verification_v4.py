#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Turbo Verification V4
# Generated: Thu May  2 11:09:37 2019
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
import mapper
import math, numpy, os
import sys
import tdd
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
        self.tdd_nullMsgSink_0 = tdd.nullMsgSink(1)
        self.tdd_ferMsgGen_0 = tdd.ferMsgGen(vcdu_size, 'cadu_len', 'vcdu_len')
        self.tdd_ferMsgCount_0 = tdd.ferMsgCount(0,0,0)
        self.mapper_prbs_source_b_0 = mapper.prbs_source_b("PRBS31", reset*8)
        self.mapper_prbs_sink_b_0 = mapper.prbs_sink_b("PRBS31", reset*8)
        self.digital_map_bb_0_0 = digital.map_bb((-1,1))
        self.ccsds_synchronizeCADUSoft_0 = ccsds.synchronizeCADUSoft('1ACFFC1D',1,7,2,7152 + 32,0,0,'syncsoft')
        self.ccsds_recoverCADUSoft_0 = ccsds.recoverCADUSoft(7152, 0, 'syncsoft')
        self.ccsds_encodeTurbo_0 = ccsds.encodeTurbo(frame_size, 4, 'cadu_len')
        self.ccsds_decodeTurbo_0 = ccsds.decodeTurbo(223, 1, 4, 1, 0.707, 0)
        self.ccsds_createCADU_0 = ccsds.createCADU(int(frame_size/Rc) + 2, '1ACFFC1D', 0, 'xx_len')
        self.blocks_unpack_k_bits_bb_1 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_debug_1 = blocks.tag_debug(gr.sizeof_float*1, '', ""); self.blocks_tag_debug_1.set_display(False)
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, int(frame_size/Rc) + 2, "xx_len")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, vcdu_size, "vcdu_len")
        self.blocks_pdu_to_tagged_stream_0_1 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'packet_len')
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_char_to_float_0_0 = blocks.char_to_float(1, 1.0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_decodeTurbo_0, 'out'), (self.tdd_ferMsgCount_0, 'in'))
        self.msg_connect((self.ccsds_decodeTurbo_0, 'out'), (self.tdd_nullMsgSink_0, 'in'))
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.ccsds_decodeTurbo_0, 'in'))
        self.msg_connect((self.tdd_ferMsgCount_0, 'out'), (self.blocks_pdu_to_tagged_stream_0_1, 'pdus'))
        self.msg_connect((self.tdd_ferMsgCount_0, 'out'), (self.tdd_nullMsgSink_0, 'in'))
        self.connect((self.blocks_char_to_float_0_0, 0), (self.ccsds_synchronizeCADUSoft_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0_1, 0), (self.blocks_unpack_k_bits_bb_1, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.tdd_ferMsgGen_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.ccsds_createCADU_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.digital_map_bb_0_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_1, 0), (self.mapper_prbs_sink_b_0, 0))
        self.connect((self.ccsds_createCADU_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.ccsds_encodeTurbo_0, 0), (self.blocks_stream_to_tagged_stream_0_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.blocks_tag_debug_1, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.ccsds_recoverCADUSoft_0, 0))
        self.connect((self.digital_map_bb_0_0, 0), (self.blocks_char_to_float_0_0, 0))
        self.connect((self.mapper_prbs_source_b_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.tdd_ferMsgGen_0, 0), (self.ccsds_encodeTurbo_0, 0))

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
        self.set_reset(self.vcdu_size)
        self.set_frame_size(self.vcdu_size + 10)
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.vcdu_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.vcdu_size)

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
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_reset(self):
        return self.reset

    def set_reset(self, reset):
        self.reset = reset

    def get_frame_size(self):
        return self.frame_size

    def set_frame_size(self, frame_size):
        self.frame_size = frame_size
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(int(self.frame_size/self.Rc) + 2)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(int(self.frame_size/self.Rc) + 2)

    def get_Rc(self):
        return self.Rc

    def set_Rc(self, Rc):
        self.Rc = Rc
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(int(self.frame_size/self.Rc) + 2)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(int(self.frame_size/self.Rc) + 2)


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
