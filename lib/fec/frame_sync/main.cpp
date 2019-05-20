#include <iostream>
#include <vector>
#include <algorithm>
#include <valarray>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "argmax.h"
using namespace std;

std::vector<float> extractSyncWord(float *in, int pos, int length);

int main()
{
    //Creating a float vector
    const int frame_size = 10000;
    float *in = (float *)malloc(frame_size*sizeof * in);
    for(int i = 0; i < frame_size; i++)
    {
        in[i] = 2*(rand() % 2) - 1;
    }

    std::vector<float> sync = extractSyncWord(in,1500,128);
    sync[0] = -1.1;
    sync[1] = 0.0;
    sync[3] = 0.9;
    sync[10]= -1.8;

    //Adding noise
    for(int i = 0; i < frame_size; i++)
    {
        in[i] += 70*(rand() % 2)/100.0;
    }

    float maxCorrelation;
    int indexMaxCorrelation;
    argmax frameSync = argmax("ABCDEFGS",2, 0);
    frameSync.setSyncWord(sync);
    frameSync.argMaxSync(in, frame_size,maxCorrelation, indexMaxCorrelation);

    //argMaxSync(sync,in, frame_size,maxCorrelation, indexMaxCorrelation);

    std::cout << "indexMaxCorr   = " << indexMaxCorrelation << std::endl;
    std::cout << "maxCorrelation = " << maxCorrelation << std::endl;
    return 0;
}

std::vector<float> extractSyncWord(float *in, int pos, int length)
{
    std::vector<float> sync;
    sync.insert(sync.end(), in + pos, in + pos + length);
    return sync;
}
