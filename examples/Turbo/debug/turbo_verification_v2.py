#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Turbo Verification V2
# Generated: Thu Feb 28 18:01:59 2019
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
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import ccsds
import dslwp
import mapper
import math, numpy
import sip
import sys
import tdd
from gnuradio import qtgui


class turbo_verification_v2(gr.top_block, Qt.QWidget):

    def __init__(self, syncword="1ACFFC1D"):
        gr.top_block.__init__(self, "Turbo Verification V2")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Turbo Verification V2")
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

        self.settings = Qt.QSettings("GNU Radio", "turbo_verification_v2")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Parameters
        ##################################################
        self.syncword = syncword

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 500e3
        self.sps = sps = 4
        self.vcdu_size = vcdu_size = 223- 10
        self.samp_rate = samp_rate = symbol_rate*sps
        self.ntaps = ntaps = 40*sps
        self.nfilts = nfilts = 32
        self.value = value = [0,1]
        self.to = to = 0.0

        self.taps = taps = firdes.root_raised_cosine(sps, samp_rate, symbol_rate, 0.35, ntaps)

        self.symbol = symbol = [-1,1]
        self.scramble = scramble = 0
        self.rs = rs = 1
        self.reset = reset = vcdu_size
        self.polyphase_taps = polyphase_taps = firdes.root_raised_cosine(sps*nfilts, nfilts, 1.0/float(sps), 0.35, 20*sps*nfilts)
        self.parity = parity = 32
        self.linecode = linecode = 1
        self.intDepth = intDepth = 1
        self.frame_size = frame_size = vcdu_size + 10
        self.cfo = cfo = 0.25
        self.SNR = SNR = 2.5
        self.Rc = Rc = 0.5

        ##################################################
        # Blocks
        ##################################################
        self.tdd_ferMsgGen_0 = tdd.ferMsgGen(vcdu_size, 'cadu_len', 'vcdu_len')
        self.tdd_ferMsgCount_0 = tdd.ferMsgCount(0)
        self.qtgui_time_sink_x_0_0 = qtgui.time_sink_f(
        	1024, #size
        	samp_rate, #samp_rate
        	"", #name
        	0 #number of inputs
        )
        self.qtgui_time_sink_x_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0.enable_control_panel(False)

        if not True:
          self.qtgui_time_sink_x_0_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_0_win)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	1024, #size
        	samp_rate, #samp_rate
        	"", #name
        	0 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)

        if not True:
          self.qtgui_time_sink_x_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.mapper_prbs_source_b_0 = mapper.prbs_source_b("PRBS31", reset*8)
        self.mapper_prbs_sink_b_0 = mapper.prbs_sink_b("PRBS31", reset*8)
        self.dslwp_frame_spliter_f_0 = dslwp.frame_spliter_f('payload_start', 3576)
        self.dslwp_ccsds_turbo_decode_0 = dslwp.ccsds_turbo_decode(223, 1, 2, 1, 0.707, 1)
        self.digital_map_bb_0_0 = digital.map_bb((-1,1))
        self.digital_map_bb_0 = digital.map_bb((-1,1))
        self.ccsds_synchronizeCADU_0 = ccsds.synchronizeCADU('1ACFFC1D', 0, "sync")
        self.ccsds_synchronizeCADUSoft_0 = ccsds.synchronizeCADUSoft('1ACFFC1D', 0, 'syncsoft')
        self.ccsds_recoverCADU_1 = ccsds.recoverCADU(int(frame_size/Rc) + 1, scramble, 'sync')
        self.ccsds_recoverCADUSoft_0 = ccsds.recoverCADUSoft(int(frame_size/Rc) + 1, scramble, 'syncsoft')
        self.ccsds_encodeTurbo_0 = ccsds.encodeTurbo(frame_size, 2, 'cadu_len')
        self.ccsds_createCADU_0 = ccsds.createCADU(int(frame_size/Rc) + 1, '1ACFFC1D', scramble, 'xx_len')
        self.blocks_unpack_k_bits_bb_1 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_0_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_debug_1 = blocks.tag_debug(gr.sizeof_float*1, '', "syncsoft"); self.blocks_tag_debug_1.set_display(False)
        self.blocks_stream_to_tagged_stream_2 = blocks.stream_to_tagged_stream(gr.sizeof_float, 1, 3576, "payload_start")
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, int(frame_size/Rc) + 1, "xx_len")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, vcdu_size, "vcdu_len")
        self.blocks_pdu_to_tagged_stream_0_1 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'packet_len')
        self.blocks_pdu_to_tagged_stream_0_0 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'packet_len')
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_char_to_float_0_0 = blocks.char_to_float(1, 1.0)
        self.blocks_char_to_float_0 = blocks.char_to_float(1, 1.0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.dslwp_ccsds_turbo_decode_0, 'in'))
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.qtgui_time_sink_x_0, 'in'))
        self.msg_connect((self.ccsds_recoverCADU_1, 'cadu'), (self.blocks_pdu_to_tagged_stream_0_0, 'pdus'))
        self.msg_connect((self.dslwp_ccsds_turbo_decode_0, 'out'), (self.tdd_ferMsgCount_0, 'in'))
        self.msg_connect((self.dslwp_frame_spliter_f_0, 'out'), (self.qtgui_time_sink_x_0_0, 'in'))
        self.msg_connect((self.tdd_ferMsgCount_0, 'out'), (self.blocks_pdu_to_tagged_stream_0_1, 'pdus'))
        self.connect((self.blocks_char_to_float_0, 0), (self.blocks_stream_to_tagged_stream_2, 0))
        self.connect((self.blocks_char_to_float_0_0, 0), (self.ccsds_synchronizeCADUSoft_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0_0, 0), (self.blocks_unpack_k_bits_bb_0_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0_1, 0), (self.blocks_unpack_k_bits_bb_1, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.tdd_ferMsgGen_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.ccsds_createCADU_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_2, 0), (self.dslwp_frame_spliter_f_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.ccsds_synchronizeCADU_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.digital_map_bb_0_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0_0, 0), (self.digital_map_bb_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_1, 0), (self.mapper_prbs_sink_b_0, 0))
        self.connect((self.ccsds_createCADU_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.ccsds_encodeTurbo_0, 0), (self.blocks_stream_to_tagged_stream_0_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.blocks_tag_debug_1, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.ccsds_recoverCADUSoft_0, 0))
        self.connect((self.ccsds_synchronizeCADU_0, 0), (self.ccsds_recoverCADU_1, 0))
        self.connect((self.digital_map_bb_0, 0), (self.blocks_char_to_float_0, 0))
        self.connect((self.digital_map_bb_0_0, 0), (self.blocks_char_to_float_0_0, 0))
        self.connect((self.mapper_prbs_source_b_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.tdd_ferMsgGen_0, 0), (self.ccsds_encodeTurbo_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "turbo_verification_v2")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_syncword(self):
        return self.syncword

    def set_syncword(self, syncword):
        self.syncword = syncword

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

    def get_vcdu_size(self):
        return self.vcdu_size

    def set_vcdu_size(self, vcdu_size):
        self.vcdu_size = vcdu_size
        self.set_reset(self.vcdu_size)
        self.set_frame_size(self.vcdu_size + 10)
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.vcdu_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.vcdu_size)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_time_sink_x_0_0.set_samp_rate(self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)
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

    def get_frame_size(self):
        return self.frame_size

    def set_frame_size(self, frame_size):
        self.frame_size = frame_size
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(int(self.frame_size/self.Rc) + 1)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(int(self.frame_size/self.Rc) + 1)

    def get_cfo(self):
        return self.cfo

    def set_cfo(self, cfo):
        self.cfo = cfo

    def get_SNR(self):
        return self.SNR

    def set_SNR(self, SNR):
        self.SNR = SNR

    def get_Rc(self):
        return self.Rc

    def set_Rc(self, Rc):
        self.Rc = Rc
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(int(self.frame_size/self.Rc) + 1)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(int(self.frame_size/self.Rc) + 1)


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    parser.add_option(
        "", "--syncword", dest="syncword", type="string", default="1ACFFC1D",
        help="Set sync [default=%default]")
    return parser


def main(top_block_cls=turbo_verification_v2, options=None):
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
