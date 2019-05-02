#ifndef REEDSOLOMON_H
#define REEDSOLOMON_H
#include <vector>
#include <algorithm>
#include <string>
#include "rs"
using namespace std;
class reedSolomon
{
public:
    //Constructors
    //reedSolomon();
    reedSolomon(int rsType, int depth, int basis);

    //Encoder and decoder
    void encodeRS223(std::vector<unsigned char> &data);
    int  decodeRS223(std::vector<unsigned char> &data);
    void encodeRS239(std::vector<unsigned char> &data);
    int  decodeRS239(std::vector<unsigned char> &data);
    void (reedSolomon::*encoder)(std::vector<unsigned char> &data);
    int  (reedSolomon::*decoder)(std::vector<unsigned char> &data);

    //Encoder and decoder wrappers
    void encode(std::vector<unsigned char> &data);
    int decode(std::vector<unsigned char> &data, int &wer);

    //Getters
    int getRSType() const;
    int getDepth()  const;
    bool getBasis()  const;
    string getRSName() const;
    int getnRoots() const;

protected:
    //Setters
    void setRSType(int rsType);
    void setDepth(int depth);
    void setBasis(int basis);

    //Basis and conventional  converters
    void basis2Conv(std::vector<unsigned char> &data);
    void conv2Basis(std::vector<unsigned char> &data);

private:
    int d_rsType;
    int d_depth;
    bool d_basis;
    string d_rsName;
    int d_nRoots;
    std::vector<int> errors;
    ezpwd::RS_CCSDS<255,223> rs223;
    ezpwd::RS_CCSDS<255,239> rs239;

};

#endif // REEDSOLOMON_H
