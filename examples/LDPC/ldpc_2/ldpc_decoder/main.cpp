#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "checknode.h"
#include "variablenode.h"
#include "alist.h"
#include <cassert>
#include "debug.h"
#include "comm.h"
#include "ldpc_encoder.h"
#include "ldpc.h"

using namespace std;
//These variables control the std::cout debug output
bool debug_level1 = false;
bool debug_level2 = false;
bool tracking = false;
bool myparity = false;
int iterations = 50;
int numPackets = 1000000;
uint64_t total_pkts = 0;
uint64_t total_errors = 0;
double maxSNR = 7;
double minSNR = 3;
int main()
{
    //C2_Parity();

    //ldpcTDD();
    std::cout << "USAGE INFORMATION" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "To enable debug information    : Set debug_level_1/debug_level_2 to true" << std::endl;
    std::cout << "To enable tracking information : Set tracking to true" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Simulation Configuration" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Number of iterations : " << iterations << std::endl;
    std::cout << "Number of packets    : " << numPackets << std::endl;
    std::cout << std::endl;

    //const string filename = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/alist/C2_Alist.a";
    //const string filename = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/LDPC/ldpc_2/ldpc_decoder/alist/C2_Alist.a";
    const string filename = "../ldpc_decoder/alist/C2_Alist.a";
    ldpc myCode(filename);
    double coderate = myCode.getCodeRate();

    // ##################################################################
    if (tracking)
        std::cout << "Creating generator matrix" << std::endl;
    std::vector < std::vector<unsigned char> > G = generatorMatrix();

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    std::vector<double> bert;
    std::vector<double> snr;
    for(int i = minSNR; i < maxSNR; i++)
        snr.push_back(i);
    for(unsigned int s = 3; s < 6; s++)
    {
        for( int x = 0; x < numPackets; x++)
        {
            // ###########################################
            if (tracking)
                std::cout << "Generating data" << std::endl;
            std::vector<unsigned char> data = data_gen(K);

            if (tracking)
                std::cout << "LDPC coding" << std::endl;
            std::vector<unsigned char> enc_data = ldpc_encode(data,G);
            if (debug_level2)
                std::cout << "Size of encoded vector : " << enc_data.size() << std::endl;
            if (tracking)
                std::cout << "BPSK modulation and awgn noise" << std::endl;
            std::vector<double> bpsk_signal =  bpsk(enc_data);
            //double snr = 4.0;
            double sigma;
            std::vector<double> noisy_signal= awgn_channel(bpsk_signal,s,coderate,sigma);
            if (debug_level2)
                std::cout << "Size of noisy_signal vector : " << noisy_signal.size() << std::endl;

            if (tracking)
                std::cout << "LDPC decoding for packet : " << x + 1 << std::endl;
            std::vector<double> logapp = myCode.decode(noisy_signal,iterations,sigma);

            //std::cout << "Size of logapp: " << logapp.size() << std::endl;
            std::vector<unsigned char> rx_bits = sliceLogAPP(logapp);
            //std::cout << "Size of data: " << data.size() << std::endl;
            int errors = ber(rx_bits,data);
            total_errors +=errors;
            total_pkts   +=K;

            //Clear all buffers
            logapp.clear();

        }
        //std::cout << "Bit Error Rate : " << double(total_errors)/double(total_pkts);
        bert.push_back(double(total_errors)/double(total_pkts));
        std::cout << std::setw(3) << s << std::setw(15) << double(total_errors)/double(total_pkts) << std::endl;
    }
    //Print BER Results
    std::cout << std::setw(3) << " SNR " << std::setw(10) << "BER" << std::endl;
    for(unsigned int i = 0; i < bert.size(); i++)
    {
        std::cout << std::setw(3) << snr[i] << std::setw(15) << bert[i] << std::endl;
    }
    return 0;
}

