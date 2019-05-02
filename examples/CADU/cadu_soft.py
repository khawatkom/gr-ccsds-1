#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Cadu Soft
# Generated: Thu Apr 18 16:10:49 2019
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
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import baseband
import bpsk
import ccsds
import channel
import mapper
import math, numpy
import pcm
import sip
import sys
import tdd
from gnuradio import qtgui


class cadu_soft(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Cadu Soft")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Cadu Soft")
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

        self.settings = Qt.QSettings("GNU Radio", "cadu_soft")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 500e3
        self.sps = sps = 4
        self.samp_rate = samp_rate = symbol_rate*sps
        self.p = p = 0.1
        self.ntaps = ntaps = 20*sps
        self.frame_size = frame_size = 501
        self.SNR = SNR = 10.0
        self.value = value = [0,1]
        self.taps_2 = taps_2 = firdes.root_raised_cosine(1, samp_rate, (samp_rate/sps), 0.35, ntaps)

        self.taps = taps = firdes.root_raised_cosine(sps, samp_rate, symbol_rate, 0.35, ntaps)

        self.symbol = symbol = [-1,1]
        self.scramble = scramble = 0
        self.rs = rs = 0
        self.reset = reset = frame_size
        self.noise_original = noise_original = math.sqrt((1)/math.pow(10,(SNR )/10.0))
        self.linecode = linecode = 0
        self.intDepth = intDepth = 1


        self.decoder = decoder = fec.cc_decoder.make((frame_size)*2*8 , 7, 2, ([79,109]), 107, -1, fec.CC_TRUNCATED, True)

        self.cfo = cfo = 0.00
        self.bn = bn = 2*math.pi * (p/100)

        ##################################################
        # Blocks
        ##################################################
        self.tdd_nullMsgSink_0 = tdd.nullMsgSink(1)
        self.qtgui_sink_x_0_0 = qtgui.sink_c(
        	1024, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/sps, #bw
        	"", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_0_win = sip.wrapinstance(self.qtgui_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_0_win)

        self.qtgui_sink_x_0_0.enable_rf_freq(False)



        self.pcm_encodeNRZ_0 = pcm.encodeNRZ(linecode)
        self.mapper_prbs_source_b_0 = mapper.prbs_source_b("PRBS31", reset*8)
        self.mapper_prbs_sink_b_0 = mapper.prbs_sink_b("PRBS31", reset*8)
        self.fir_filter_xxx_0 = filter.fir_filter_ccc(sps, (taps))
        self.fir_filter_xxx_0.declare_sample_delay(0)
        self.digital_binary_slicer_fb_0 = digital.binary_slicer_fb()
        self.channel_phaseError_0 = channel.phaseError(135)
        self.channel_gsPhaseNoise_0 = channel.gsPhaseNoise(math.sqrt(samp_rate/2), ( [2.800000000000000e-06,-5.866548800000000e-06,3.064241879202000e-06]), ( [1,-2.555230500000000,2.114073646727000,-0.558843141309031] ))
        self.ccsds_synchronizeCADUSoft_0 = ccsds.synchronizeCADUSoft('1ACFFC1D',1,7,0,(frame_size + 4)*8,0,0,'sync')
        self.ccsds_recoverCADUSoft_0 = ccsds.recoverCADUSoft((frame_size)*8, scramble, 'sync')
        self.ccsds_createCADU_0 = ccsds.createCADU(frame_size, '1ACFFC1D', scramble, 'packet_len')
        self.bpsk_bpskPulseshapeRRC_0 = bpsk.bpskPulseshapeRRC(sps, 1.0, samp_rate, samp_rate/(sps), 0.35, 40*sps)
        self.bpsk_bpskPhaseRecovery_0 = bpsk.bpskPhaseRecovery(4, bn, math.sqrt(2)/2, 2)
        self.bpsk_bpskIQMap_0 = bpsk.bpskIQMap()
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, frame_size, "packet_len")
        self.blocks_pdu_to_tagged_stream_0 = blocks.pdu_to_tagged_stream(blocks.float_t, 'packet_len')
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((-1, ))
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.baseband_debug_channel_model_1_0 = baseband.debug_channel_model_1(noise_original/math.sqrt(sps), cfo, 1.0, (1.0 , ), 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.blocks_pdu_to_tagged_stream_0, 'pdus'))
        self.msg_connect((self.ccsds_recoverCADUSoft_0, 'cadu'), (self.tdd_nullMsgSink_0, 'in'))
        self.connect((self.baseband_debug_channel_model_1_0, 0), (self.channel_gsPhaseNoise_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.ccsds_synchronizeCADUSoft_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_complex_to_real_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_pdu_to_tagged_stream_0, 0), (self.digital_binary_slicer_fb_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.ccsds_createCADU_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.pcm_encodeNRZ_0, 0))
        self.connect((self.bpsk_bpskIQMap_0, 0), (self.channel_phaseError_0, 0))
        self.connect((self.bpsk_bpskPhaseRecovery_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.bpsk_bpskPhaseRecovery_0, 0), (self.qtgui_sink_x_0_0, 0))
        self.connect((self.bpsk_bpskPulseshapeRRC_0, 0), (self.bpsk_bpskIQMap_0, 0))
        self.connect((self.ccsds_createCADU_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.ccsds_synchronizeCADUSoft_0, 0), (self.ccsds_recoverCADUSoft_0, 0))
        self.connect((self.channel_gsPhaseNoise_0, 0), (self.fir_filter_xxx_0, 0))
        self.connect((self.channel_phaseError_0, 0), (self.baseband_debug_channel_model_1_0, 0))
        self.connect((self.digital_binary_slicer_fb_0, 0), (self.mapper_prbs_sink_b_0, 0))
        self.connect((self.fir_filter_xxx_0, 0), (self.bpsk_bpskPhaseRecovery_0, 0))
        self.connect((self.mapper_prbs_source_b_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.pcm_encodeNRZ_0, 0), (self.bpsk_bpskPulseshapeRRC_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "cadu_soft")
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
        self.set_taps_2(firdes.root_raised_cosine(1, self.samp_rate, (self.samp_rate/self.sps), 0.35, self.ntaps))
        self.qtgui_sink_x_0_0.set_frequency_range(0, self.samp_rate/self.sps)
        self.set_ntaps(20*self.sps)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_taps_2(firdes.root_raised_cosine(1, self.samp_rate, (self.samp_rate/self.sps), 0.35, self.ntaps))
        self.qtgui_sink_x_0_0.set_frequency_range(0, self.samp_rate/self.sps)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_p(self):
        return self.p

    def set_p(self, p):
        self.p = p
        self.set_bn(2*math.pi * (self.p/100))

    def get_ntaps(self):
        return self.ntaps

    def set_ntaps(self, ntaps):
        self.ntaps = ntaps
        self.set_taps_2(firdes.root_raised_cosine(1, self.samp_rate, (self.samp_rate/self.sps), 0.35, self.ntaps))

    def get_frame_size(self):
        return self.frame_size

    def set_frame_size(self, frame_size):
        self.frame_size = frame_size
        self.set_reset(self.frame_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.frame_size)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.frame_size)

    def get_SNR(self):
        return self.SNR

    def set_SNR(self, SNR):
        self.SNR = SNR
        self.set_noise_original(math.sqrt((1)/math.pow(10,(self.SNR )/10.0)))

    def get_value(self):
        return self.value

    def set_value(self, value):
        self.value = value

    def get_taps_2(self):
        return self.taps_2

    def set_taps_2(self, taps_2):
        self.taps_2 = taps_2

    def get_taps(self):
        return self.taps

    def set_taps(self, taps):
        self.taps = taps
        self.fir_filter_xxx_0.set_taps((self.taps))

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

    def get_noise_original(self):
        return self.noise_original

    def set_noise_original(self, noise_original):
        self.noise_original = noise_original

    def get_linecode(self):
        return self.linecode

    def set_linecode(self, linecode):
        self.linecode = linecode

    def get_intDepth(self):
        return self.intDepth

    def set_intDepth(self, intDepth):
        self.intDepth = intDepth

    def get_decoder(self):
        return self.decoder

    def set_decoder(self, decoder):
        self.decoder = decoder

    def get_cfo(self):
        return self.cfo

    def set_cfo(self, cfo):
        self.cfo = cfo

    def get_bn(self):
        return self.bn

    def set_bn(self, bn):
        self.bn = bn


def main(top_block_cls=cadu_soft, options=None):

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
