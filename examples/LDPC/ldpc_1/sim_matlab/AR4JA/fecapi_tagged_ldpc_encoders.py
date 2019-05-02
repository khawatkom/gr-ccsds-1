#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Fecapi Tagged Ldpc Encoders
# Generated: Wed Mar 20 19:09:34 2019
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
from gnuradio import eng_notation
from gnuradio import fec
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import sys
from gnuradio import qtgui


class fecapi_tagged_ldpc_encoders(gr.top_block, Qt.QWidget):

    def __init__(self, puncpat='11'):
        gr.top_block.__init__(self, "Fecapi Tagged Ldpc Encoders")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Fecapi Tagged Ldpc Encoders")
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

        self.settings = Qt.QSettings("GNU Radio", "fecapi_tagged_ldpc_encoders")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Parameters
        ##################################################
        self.puncpat = puncpat

        ##################################################
        # Variables
        ##################################################
        self.G = G = fec.ldpc_G_matrix('/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/LDPC/sim_matlab/AR4JA/alist/AR4JA_r12_k1024n.alist')
        self.samp_rate = samp_rate = 50000


        self.ldpc_enc_G = ldpc_enc_G = fec.ldpc_gen_mtrx_encoder_make(G)
        self.frame_size_H = frame_size_H = 42
        self.frame_size_G = frame_size_G = 1024
        self.MTU_H = MTU_H = 1512
        self.MTU_G = MTU_G = 1508

        ##################################################
        # Blocks
        ##################################################

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "fecapi_tagged_ldpc_encoders")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_puncpat(self):
        return self.puncpat

    def set_puncpat(self, puncpat):
        self.puncpat = puncpat

    def get_G(self):
        return self.G

    def set_G(self, G):
        self.G = G

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_ldpc_enc_G(self):
        return self.ldpc_enc_G

    def set_ldpc_enc_G(self, ldpc_enc_G):
        self.ldpc_enc_G = ldpc_enc_G

    def get_frame_size_H(self):
        return self.frame_size_H

    def set_frame_size_H(self, frame_size_H):
        self.frame_size_H = frame_size_H

    def get_frame_size_G(self):
        return self.frame_size_G

    def set_frame_size_G(self, frame_size_G):
        self.frame_size_G = frame_size_G

    def get_MTU_H(self):
        return self.MTU_H

    def set_MTU_H(self, MTU_H):
        self.MTU_H = MTU_H

    def get_MTU_G(self):
        return self.MTU_G

    def set_MTU_G(self, MTU_G):
        self.MTU_G = MTU_G


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    parser.add_option(
        "", "--puncpat", dest="puncpat", type="string", default='11',
        help="Set puncpat [default=%default]")
    return parser


def main(top_block_cls=fecapi_tagged_ldpc_encoders, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls(puncpat=options.puncpat)
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
