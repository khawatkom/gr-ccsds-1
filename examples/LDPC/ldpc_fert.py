#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Ldpc Fert
# Generated: Mon Apr 29 16:06:31 2019
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
import bpsk
import ccsds
import mapper
import math, numpy
import numpy
import sys
import tdd
from gnuradio import qtgui


class ldpc_fert(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Ldpc Fert")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Ldpc Fert")
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

        self.settings = Qt.QSettings("GNU Radio", "ldpc_fert")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 5000e3
        self.sps = sps = 4
        self.N = N = 8160
        self.K = K = 7136
        self.samp_rate = samp_rate = symbol_rate*sps
        self.ntaps = ntaps = 10*sps
        self.nfilts = nfilts = 32
        self.Rm = Rm = 1
        self.Rc = Rc = K*1.0/N*1.0
        self.EbNo = EbNo = 4.0
        self.voltage_0 = voltage_0 = (math.sqrt(2)/math.sqrt(2*Rm*Rc*math.pow(10.0,EbNo/10.0)))/math.sqrt(sps)
        self.vcdu_size = vcdu_size = 7136
        self.value = value = [0,1]

        self.taps = taps = firdes.root_raised_cosine(sps, samp_rate, symbol_rate, 0.35, ntaps)

        self.symbol = symbol = [-1,1]
        self.sigma = sigma = math.sqrt(1.0/(2*math.pow(10.0,EbNo/10.0)))
        self.scramble = scramble = 0
        self.rs = rs = 1
        self.polyphase_taps = polyphase_taps = firdes.root_raised_cosine(sps*nfilts, nfilts, 1.0/float(sps), 0.35, 20*sps*nfilts)
        self.parity = parity = 32
        self.linecode = linecode = 1
        self.intDepth = intDepth = 1

        self.const = const = digital.constellation_bpsk().base()

        self.const.gen_soft_dec_lut(4)
        self.cadu_size = cadu_size = N/8

        ##################################################
        # Blocks
        ##################################################
        self.tdd_nullMsgSink_0 = tdd.nullMsgSink(1)
        self.tdd_ferMsgGen_0 = tdd.ferMsgGen(882, '', 'pkt_len')
        self.tdd_ferMsgCount_0 = tdd.ferMsgCount(1,0,1)
        self.mapper_prbs_sink_b_0 = mapper.prbs_sink_b("PRBS31", 7056)
        self.fir_filter_xxx_0 = filter.fir_filter_ccc(sps, (taps))
        self.fir_filter_xxx_0.declare_sample_delay(0)
        self.ccsds_synchronizeCADUSoft_0 = ccsds.synchronizeCADUSoft('1ACFFC1D',1,7,0,N + 32,0,0,'sync')
        self.ccsds_recoverCADUSoft_0 = ccsds.recoverCADUSoft(N, 1, 'sync')
        self.ccsds_encodeLDPC_0 = ccsds.encodeLDPC("empy.txt",1,'cadu_len',"vcdu_len",0)
        self.ccsds_decodeLDPC_0 = ccsds.decodeLDPC("/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/alist/AR4JA_r12_k1024n.a",1,20, sigma, 1,1)
        self.ccsds_createCADU_0 = ccsds.createCADU(cadu_size, '1ACFFC1D', 1, 'cadu_len')
        self.bpsk_bpskSoftDecision_0 = bpsk.bpskSoftDecision(10, 2, 1)
        self.bpsk_bpskPulseshapeRRC_0 = bpsk.bpskPulseshapeRRC(sps, 1, samp_rate, symbol_rate, 0.35, ntaps)
        self.bpsk_bpskIQMap_0 = bpsk.bpskIQMap()
        self.blocks_unpack_k_bits_bb_2 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_1 = blocks.unpack_k_bits_bb(8)
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_float*1, '', ""); self.blocks_tag_debug_0.set_display(False)
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, 7136, 'vcdu_len')
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, 7056, 'pkt_len')
        self.blocks_pdu_to_tagged_stream_0_0 = blocks.pdu_to_tagged_stream(blocks.byte_t, 'length_tag')
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_random_source_x_0 = blocks.vector_source_b(map(int, numpy.random.randint(0, 2, 7056)), True)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, (math.sqrt(2)/math.sqrt(2*Rm*Rc*math.pow(10.0,EbNo/10.0)))/math.sqrt(sps), 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_decodeLDPC_0, 'out'), (self.tdd_ferMsgCount_0, 'in'))
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.ccsds_decodeLDPC_0, 'in'))
        self.msg_connect((self.tdd_ferMsgCount_0, 'out'), (self.blocks_pdu_to_tagged_stream_0_0, 'pdus'))
        self.msg_connect((self.tdd_ferMsgCount_0, 'out'), (self.tdd_nullMsgSink_0, 'in'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.analog_random_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.fir_filter_xxx_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.tdd_ferMsgGen_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0_0, 0), (self.blocks_unpack_k_bits_bb_1, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.ccsds_encodeLDPC_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.bpsk_bpskPulseshapeRRC_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_1, 0), (self.mapper_prbs_sink_b_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_2, 0), (self.blocks_stream_to_tagged_stream_0_0, 0))
        self.connect((self.bpsk_bpskIQMap_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.bpsk_bpskPulseshapeRRC_0, 0), (self.bpsk_bpskIQMap_0, 0))
        self.connect((self.bpsk_bpskSoftDecision_0, 0), (self.ccsds_synchronizeCADUSoft_0, 0))
        self.connect((self.ccsds_createCADU_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.ccsds_encodeLDPC_0, 0), (self.ccsds_createCADU_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.blocks_tag_debug_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.ccsds_recoverCADUSoft_0, 0))
        self.connect((self.fir_filter_xxx_0, 0), (self.bpsk_bpskSoftDecision_0, 0))
        self.connect((self.tdd_ferMsgGen_0, 0), (self.blocks_unpack_k_bits_bb_2, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "ldpc_fert")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

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
        self.set_ntaps(10*self.sps)
        self.set_voltage_0((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))
        self.set_polyphase_taps(firdes.root_raised_cosine(self.sps*self.nfilts, self.nfilts, 1.0/float(self.sps), 0.35, 20*self.sps*self.nfilts))
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_N(self):
        return self.N

    def set_N(self, N):
        self.N = N
        self.set_cadu_size(self.N/8)
        self.set_Rc(self.K*1.0/self.N*1.0)

    def get_K(self):
        return self.K

    def set_K(self, K):
        self.K = K
        self.set_Rc(self.K*1.0/self.N*1.0)

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

    def get_Rm(self):
        return self.Rm

    def set_Rm(self, Rm):
        self.Rm = Rm
        self.set_voltage_0((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_Rc(self):
        return self.Rc

    def set_Rc(self, Rc):
        self.Rc = Rc
        self.set_voltage_0((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_EbNo(self):
        return self.EbNo

    def set_EbNo(self, EbNo):
        self.EbNo = EbNo
        self.set_sigma(math.sqrt(1.0/(2*math.pow(10.0,self.EbNo/10.0))))
        self.set_voltage_0((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))
        self.analog_noise_source_x_0.set_amplitude((math.sqrt(2)/math.sqrt(2*self.Rm*self.Rc*math.pow(10.0,self.EbNo/10.0)))/math.sqrt(self.sps))

    def get_voltage_0(self):
        return self.voltage_0

    def set_voltage_0(self, voltage_0):
        self.voltage_0 = voltage_0

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

    def get_rs(self):
        return self.rs

    def set_rs(self, rs):
        self.rs = rs

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

    def get_const(self):
        return self.const

    def set_const(self, const):
        self.const = const

    def get_cadu_size(self):
        return self.cadu_size

    def set_cadu_size(self, cadu_size):
        self.cadu_size = cadu_size


def main(top_block_cls=ldpc_fert, options=None):

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
