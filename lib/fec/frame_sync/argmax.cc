#include "argmax.h"

argmax::argmax(std::string syncword, int matchingFuntion, int ambiguity)
{
    switch(matchingFuntion)
    {
    case 0:
        d_mtxFunction = &argmax::hardCorr;
        break;
    case 1:
        d_mtxFunction = &argmax::softCorr;
        break;
    case 2:
        d_mtxFunction = &argmax::masseyCorr;
        break;
    default:
        d_mtxFunction = &argmax::hardCorr;
    }
    //Setting sync word
    std::string ASM = hex_str_to_bin_str(syncword);
    for(int i = 0; i < ASM.length();i++)
        d_sync.push_back((ASM[i] == '1') ? 1.0 : -1.0);
    std::cout << "Length of float asm pattern" << d_sync.size() << std::endl;
}
void
argmax::setSyncWord(std::vector<float> sync)
{
    d_sync = sync;
    return;
}

std::vector<float>
argmax::extractSyncWord(float *in, int pos, int length)
{
    std::vector<float> sync;
    sync.insert(sync.end(), in + pos, in + pos + length);
    return sync;
}

void
argmax::printVector(std::vector<float> in)
{
    for(unsigned int i = 0; i < in.size(); i++)
        std::cout << in[i] << " ";
    std::cout << std::endl;
}
float
argmax::binarySlice(float in)
{
    return (in >=0.0 ) ? 1.0 : -1.0;
}

float
argmax::softCorr(std::vector<float> in0,std::vector<float> in1)
{
   assert(in0.size() == in1.size());
   std::valarray<float> arr0(in0.data(),in0.size());
   std::valarray<float> arr1(in1.data(),in1.size());
   std::valarray<float> corr = arr0*arr1;

   return corr.sum();
}

float
argmax::hardCorr(std::vector<float> in0,std::vector<float> in1)
{
    assert(in0.size() == in1.size());
    std::valarray<float> corr(0.0,in0.size());
    for(unsigned int i = 0 ; i < in0.size(); i++)
       corr[i] = (in0[i]*binarySlice(in1[i]));
    return corr.sum();
}

float
argmax::masseyCorr(std::vector<float> in0,std::vector<float> in1)
{
    std::valarray<float> corr(0.0,in0.size());
    for(unsigned int i = 0 ; i < in0.size(); i++)
       corr[i] = (binarySlice(in1[i]) == in0[i]) ? 0.0 : -abs(in1[i]);
    return corr.sum();
}

void
argmax::argMaxSync(float *in, int frame_length,
                   float &maxCorrelation, int &indexMaxCorrelation)
{
    std::vector<float> corr;
    for(unsigned int i = 0; i < frame_length - d_sync.size(); i++)
    {
        corr.push_back((*this.*d_mtxFunction)(d_sync,extractSyncWord(in,i,d_sync.size())));
    }
    indexMaxCorrelation = std::distance(corr.begin(), std::max_element(corr.begin(), corr.end()));
    maxCorrelation = corr[indexMaxCorrelation];
    return;
}

std::string
argmax::hex_str_to_bin_str(const std::string& hex)
{
    // TODO use a loop from <algorithm> or smth
    std::string bin;
    for(unsigned i = 0; i != hex.length(); ++i)
        bin += hex_char_to_bin(hex[i]);
    return bin;
}

const char*
argmax::hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    default:
        return "0000";
    }
}
