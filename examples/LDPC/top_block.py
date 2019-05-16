#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Wed May 15 02:02:12 2019
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
import math, numpy
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
        self.symbol_rate = symbol_rate = 50e3
        self.sps = sps = 4
        self.samp_rate = samp_rate = symbol_rate*sps
        self.ntaps = ntaps = 10*sps
        self.N = N = 8160 + 18 - 2
        self.K = K = 7136 + 18
        self.EbNo = EbNo = 4.0
        self.vcdu_size = vcdu_size = 7136
        self.value = value = [0,1]

        self.taps = taps = firdes.root_raised_cosine(sps, samp_rate, symbol_rate, 0.35, ntaps)

        self.symbol = symbol = [-1,1]
        self.sigma = sigma = math.sqrt(1.0/(2*math.pow(10.0,EbNo/10.0)))
        self.scramble = scramble = 0
        self.cadu_size = cadu_size = N/8
        self.Rm = Rm = 1
        self.Rc = Rc = K*1.0/N*1.0

        ##################################################
        # Blocks
        ##################################################
        self.root_raised_cosine_filter_0 = filter.interp_fir_filter_fff(sps, firdes.root_raised_cosine(
        	1, samp_rate, symbol_rate, 0.35, ntaps))
        self.mapper_prbs_source_b_0 = mapper.prbs_source_b("PRBS31", K)
        self.mapper_prbs_sink_b_0 = mapper.prbs_sink_b("PRBS31", K)
        self.fir_filter_xxx_0 = filter.fir_filter_ccc(sps, (taps))
        self.fir_filter_xxx_0.declare_sample_delay(0)
        self.digital_map_bb_0 = digital.map_bb((-1,1))
        self.ccsds_encodeLDPC_0 = ccsds.encodeLDPC('/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/gmini/C2.txt',0,'cadu_len',"vcdu_len",0)
        self.ccsds_createCADU_0 = ccsds.createCADU(cadu_size, '1ACFFC1D', 1, 'cadu_len')
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tagged_stream_to_pdu_0 = blocks.tagged_stream_to_pdu(blocks.float_t, 'packet_len')
        self.blocks_stream_to_tagged_stream_1 = blocks.stream_to_tagged_stream(gr.sizeof_float, 1, N, "packet_len")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, K, "vcdu_len")
        self.blocks_pdu_to_tagged_stream_0_0 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'length_tag')
        self.blocks_null_source_0 = blocks.null_source(gr.sizeof_float*1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.blocks_char_to_float_0 = blocks.char_to_float(1, 1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, (math.sqrt(2)/math.sqrt(2*Rm*Rc*math.pow(10.0,EbNo/10.0)))/math.sqrt(sps), 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_tagged_stream_to_pdu_0, 'pdus'), (self.blocks_pdu_to_tagged_stream_0_0, 'pdus'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.fir_filter_xxx_0, 0))
        self.connect((self.blocks_char_to_float_0, 0), (self.root_raised_cosine_filter_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.blocks_stream_to_tagged_stream_1, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_pdu_to_tagged_stream_0_0, 0), (self.mapper_prbs_sink_b_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.ccsds_encodeLDPC_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_1, 0), (self.blocks_tagged_stream_to_pdu_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.digital_map_bb_0, 0))
        self.connect((self.ccsds_createCADU_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.ccsds_encodeLDPC_0, 0), (self.ccsds_createCADU_0, 0))
        self.connect((self.digital_map_bb_0, 0), (self.blocks_char_to_float_0, 0))
        self.connect((self.fir_filter_xxx_0, 0), (self.blocks_complex_to_real_0, 0))
        self.connect((self.mapper_prbs_source_b_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.root_raised_cosine_filter_0, 0), (self.blocks_float_to_complex_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_samp_rate(self.symbol_rate*self.sps)
        self.root_raised_cosine_filter_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.set_samp_rate(self.symbol_rate*self.sps)
        self.set_ntaps(10*self.sps)
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.root_raised_cosine_filter_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_ntaps(self):
        return self.ntaps

    def set_ntaps(self, ntaps):
        self.ntaps = ntaps
        self.root_raised_cosine_filter_0.set_taps(firdes.root_raised_cosine(1, self.samp_rate, self.symbol_rate, 0.35, self.ntaps))

    def get_N(self):
        return self.N

    def set_N(self, N):
        self.N = N
        self.set_cadu_size(self.N/8)
        self.set_Rc(self.K*1.0/self.N*1.0)
        self.blocks_stream_to_tagged_stream_1.set_packet_len(self.N)
        self.blocks_stream_to_tagged_stream_1.set_packet_len_pmt(self.N)

    def get_K(self):
        return self.K

    def set_K(self, K):
        self.K = K
        self.set_Rc(self.K*1.0/self.N*1.0)
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.K)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.K)

    def get_EbNo(self):
        return self.EbNo

    def set_EbNo(self, EbNo):
        self.EbNo = EbNo
        self.set_sigma(math.sqrt(1.0/(2*math.pow(10.0,self.EbNo/10.0))))
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_vcdu_size(self):
        return self.vcdu_size

    def set_vcdu_size(self, vcdu_size):
        self.vcdu_size = vcdu_size

    def get_value(self):
        return self.value

    def set_value(self, value):
        self.value = value

    def get_taps(self):
        return self.taps

    def set_taps(self, taps):
        self.taps = taps
        self.fir_filter_xxx_0.set_taps((self.taps))

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

    def get_cadu_size(self):
        return self.cadu_size

    def set_cadu_size(self, cadu_size):
        self.cadu_size = cadu_size

    def get_Rm(self):
        return self.Rm

    def set_Rm(self, Rm):
        self.Rm = Rm
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_Rc(self):
        return self.Rc

    def set_Rc(self, Rc):
        self.Rc = Rc
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))


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
