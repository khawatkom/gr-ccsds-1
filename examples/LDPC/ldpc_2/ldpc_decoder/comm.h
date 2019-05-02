#ifndef COMM
#define COMM
#include <vector>
#include <algorithm>
#include <cstdlib>

std::vector<unsigned char> data_gen(unsigned int nBits)
{
    std::vector<unsigned char> bits;
    int temp;
    for(unsigned int i = 0; i < nBits;i++){
        temp = (((rand()%2) - 0.5)*2.0);
        bits.push_back( temp > 0.0 ? 0x01 : 0x00);
        //std::cout << temp;
    }
    return bits;
}

std::vector<double> randn(double mean, double variance, unsigned int length)
{
    /*{{{*/
    //double* random =  malloc(length * sizeof *random);
    std::vector<double> awgn;

    for (unsigned int i = 0; i < length; i++)
    {
        double U1 = ((double)rand()/(double)RAND_MAX);
        double U2 = ((double)rand()/(double)RAND_MAX);

        double R = sqrt(-2*variance*log(U1));
        double theta = 2*M_PI*U2;

        awgn.push_back( mean + R*cos(theta));
    }

    return awgn;/*}}}*/
}


std::vector<double> bpsk(std::vector<unsigned char> data)
{
    std::vector<double> signal;
    for(unsigned int i = 0; i < data.size();i++)
        signal.push_back((data[i] > 0) ? 1.0000 : -1.0000);
    return signal;
}

std::vector<double> awgn_channel(std::vector<double> signal, double snr, double code_rate, double &sigma)
{
    std::vector<double> noisy_signal;
    std::vector<double> awgn = randn(0,1,signal.size());
    double N0 = pow(10.0, -snr / 10.0) / code_rate;
    sigma = sqrt(N0 / 2.0);
    for(unsigned int i = 0; i < signal.size();i++)
    {
        noisy_signal.push_back(signal[i] + sigma*awgn[i]);
    }
    return noisy_signal;
}

int ber(std::vector<unsigned char> tx,std::vector<unsigned char> rx )
{
    assert(tx.size() == rx.size());
    int numErrors = 0;
    for(unsigned int i = 0; i<tx.size();i++)
    {
        if (tx[i] == rx[i])
              numErrors++;
    }
    return numErrors;
}

std::vector<unsigned char> sliceLogAPP(std::vector<double> logapp)
{
    std::vector<unsigned char> bits;
    for(unsigned int i = 0; i < logapp.size(); i++)
        bits.push_back(logapp[i] > 0.0 ? 0x00 : 0x01);
    return bits;
}


#endif // COMM

