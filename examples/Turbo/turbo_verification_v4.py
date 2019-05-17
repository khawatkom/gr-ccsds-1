#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Turbo Verification V4
# Generated: Fri May 17 02:40:36 2019
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
from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import ccsds
import mapper
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
        self.vcdu_size = vcdu_size = 223
        self.symbol_rate = symbol_rate = 2e6
        self.sps = sps = 4
        self.EbNo = EbNo = 10.0
        self.value = value = [0,1]
        self.symbol = symbol = [-1,1]
        self.sigma = sigma = math.sqrt(1.0/(2*math.pow(10.0,EbNo/10.0)))
        self.scramble = scramble = 0
        self.samp_rate = samp_rate = symbol_rate*sps
        self.reset = reset = vcdu_size
        self.ntaps = ntaps = 10*sps
        self.frame_size = frame_size = vcdu_size
        self.Rm = Rm = 1
        self.Rc = Rc = 0.25

        ##################################################
        # Blocks
        ##################################################
        self.root_raised_cosine_filter_0_0 = filter.fir_filter_ccf(sps, firdes.root_raised_cosine(
        	1, samp_rate, symbol_rate, 0.35, ntaps))
        self.root_raised_cosine_filter_0 = filter.interp_fir_filter_fff(sps, firdes.root_raised_cosine(
        	1, samp_rate, symbol_rate, 0.35, ntaps))
        self.mapper_prbs_source_b_0 = mapper.prbs_source_b("PRBS31", reset*8)
        self.mapper_prbs_sink_b_0 = mapper.prbs_sink_b("PRBS31", reset*8)
        self.digital_map_bb_0_0 = digital.map_bb((-1,1))
        self.ccsds_synchronizeCADUSoft_0 = ccsds.synchronizeCADUSoft('1ACFFC1D',1,7,0,7152 + 32,0,0,'syncsoft')
        self.ccsds_recoverCADUSoft_0 = ccsds.recoverCADUSoft(7152, 0, 'syncsoft')
        self.ccsds_encodeTurbo_0 = ccsds.encodeTurbo(frame_size, 4, "vcdu_len")
        self.ccsds_decodeTurbo_0 = ccsds.decodeTurbo(223, 1, 4, 1, sigma, 1)
        self.ccsds_createCADU_0 = ccsds.createCADU(int(frame_size/Rc) + 2, '1ACFFC1D', 0, 'xx_len')
        self.blocks_unpack_k_bits_bb_1 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_debug_1 = blocks.tag_debug(gr.sizeof_float*1, '', "syncsoft"); self.blocks_tag_debug_1.set_display(False)
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, int(frame_size/Rc) + 2, "xx_len")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, vcdu_size, "vcdu_len")
        self.blocks_pdu_to_tagged_stream_0_1 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'packet_len')
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_null_source_0 = blocks.null_source(gr.sizeof_float*1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.blocks_char_to_float_0_0 = blocks.char_to_float(1, 1.0)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, (math.sqrt(2)/math.sqrt(2*Rm*Rc*math.pow(10.0,EbNo/10.0)))/math.sqrt(sps), 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_decodeTurbo_0, 'out'), (self.blocks_pdu_to_tagged_stream_0_1, 'pdus'))
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.ccsds_decodeTurbo_0, 'in'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.root_raised_cosine_filter_0_0, 0))
        self.connect((self.blocks_char_to_float_0_0, 0), (self.root_raised_cosine_filter_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.ccsds_synchronizeCADUSoft_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0_1, 0), (self.blocks_unpack_k_bits_bb_1, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.ccsds_encodeTurbo_0, 0))
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
        self.connect((self.root_raised_cosine_filter_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.root_raised_cosine_filter_0_0, 0), (self.blocks_complex_to_real_0, 0))

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
        self.set_frame_size(self.vcdu_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.vcdu_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.vcdu_size)

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_samp_rate(self.symbol_rate*self.sps)
        self.root_raised_cosine_filter_0_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))
        self.root_raised_cosine_filter_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.set_samp_rate(self.symbol_rate*self.sps)
        self.set_ntaps(10*self.sps)
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_EbNo(self):
        return self.EbNo

    def set_EbNo(self, EbNo):
        self.EbNo = EbNo
        self.set_sigma(math.sqrt(1.0/(2*math.pow(10.0,self.EbNo/10.0))))
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_value(self):
        return self.value

    def set_value(self, value):
        self.value = value

    def get_symbol(self):
        return self.symbol

    def set_symbol(self, symbol):
        self.symbol = symbol

    def get_sigma(self):
        return self.sigma

    def set_sigma(self, sigma):
        self.sigma = sigma

    def get_scramble(self):
        return self.scramble

    def set_scramble(self, scramble):
        self.scramble = scramble

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.root_raised_cosine_filter_0_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))
        self.root_raised_cosine_filter_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_reset(self):
        return self.reset

    def set_reset(self, reset):
        self.reset = reset

    def get_ntaps(self):
        return self.ntaps

    def set_ntaps(self, ntaps):
        self.ntaps = ntaps
        self.root_raised_cosine_filter_0_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))
        self.root_raised_cosine_filter_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))

    def get_frame_size(self):
        return self.frame_size

    def set_frame_size(self, frame_size):
        self.frame_size = frame_size
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(int(self.frame_size/self.Rc) + 2)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(int(self.frame_size/self.Rc) + 2)

    def get_Rm(self):
        return self.Rm

    def set_Rm(self, Rm):
        self.Rm = Rm
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_Rc(self):
        return self.Rc

    def set_Rc(self, Rc):
        self.Rc = Rc
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(int(self.frame_size/self.Rc) + 2)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(int(self.frame_size/self.Rc) + 2)
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))


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
