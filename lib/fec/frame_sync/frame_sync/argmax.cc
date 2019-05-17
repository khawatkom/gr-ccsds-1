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
    //Setting sync word according to ambiguity resolution
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
