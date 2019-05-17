#ifndef ARGMAX_H
#define ARGMAX_H
#include <iostream>
#include <vector>
#include <valarray>
#include <algorithm>
#include <cassert>
#include <string>

class argmax
{
public:
    argmax(std::string syncword,int matchingFuntion, int ambiguity);
    void setSyncWord(std::vector<float> sync);
    void argMaxSync(float *in,
                    int frame_length, float &maxCorrelation, int &indexMaxCorrelation);
private:
    float (argmax::*d_mtxFunction)(std::vector<float> in0,std::vector<float> in1);
    std::vector<float> d_sync;
    std::vector<float> extractSyncWord(float *in, int pos, int length);
    void printVector(std::vector<float> in);
    float hardCorr(std::vector<float> in0,std::vector<float> in1);
    float softCorr(std::vector<float> in0,std::vector<float> in1);
    float masseyCorr(std::vector<float> in0,std::vector<float> in1);
    float binarySlice(float in);
};

#endif // ARGMAX_H
