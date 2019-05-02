#include "reedsolomon.h"
#include "basis.h"

/*reedSolomon::reedSolomon()
{

}*/

reedSolomon::reedSolomon(int rsType, int depth, int basis)
{
    //Reed-Solomon parameters
    setRSType(rsType);
    setDepth(depth);
    setBasis(basis);

    //Selecting CCSDS encoder. 0 - RS(255,223), 1 - RS(255,239)
    switch(d_rsType)
    {
    case 0:
        encoder = &reedSolomon::encodeRS223;
        decoder = &reedSolomon::decodeRS223;
        d_rsName =  "CCSDS(255,223)";
        d_nRoots = rs223.nroots();
        break;
    case 1:
        encoder = &reedSolomon::encodeRS239;
        decoder = &reedSolomon::decodeRS239;
        d_rsName =  "CCSDS(255,239)";
        d_nRoots = rs239.nroots();
        break;
    default:
        encoder = &reedSolomon::encodeRS223;
        decoder = &reedSolomon::decodeRS223;
        d_rsName =  "CCSDS(255,223)";
        d_nRoots = rs223.nroots();
    }
}

void
reedSolomon::encode(std::vector<unsigned char> &data)
{
    if(getDepth() == 1)
    {
        (*this.*encoder)(data);
        return;
    }//
    else{
        //Interleave and RS coding
        std::vector<unsigned char> interleaver;
        std::vector<unsigned char> buffer;
        for(int j = 0; j < getDepth();j++)
        {
            for(int i = 0; i < data.size();i+=getDepth())
            {
                buffer.push_back(data.at(i + j));
            }
            (*this.*encoder)(buffer);
            interleaver.insert(interleaver.end(),buffer.begin(),buffer.end());
            buffer.clear();
        }

        //De-interleaving
        std::vector<unsigned char> deinterleaver;
        for(int j = 0; j < interleaver.size()/getDepth(); j++)
        {
            for(int i = 0; i < interleaver.size(); i += interleaver.size()/getDepth())
            {
                deinterleaver.push_back(interleaver.at(i + j));
            }
        }
        data = deinterleaver;
        deinterleaver.clear();
        return;
    }
}

int
reedSolomon::decode(std::vector<unsigned char> &data,int &wer)
{
    int errors;
    wer = 0;
    std::vector<int> block_errors;
    //return (*this.*decoder)(data);

    if(getDepth() == 1)
    {
        errors = (*this.*decoder)(data);
        data.resize(data.size() - getnRoots());
         wer += (errors < 0) ? 1 : 0;
        return errors;
    }
    else{
        //Interleave and RS coding
        std::vector<unsigned char> interleaver;
        std::vector<unsigned char> buffer;

        for(int j = 0; j < getDepth();j++)
        {
            for(int i = 0; i < data.size();i+=getDepth())
            {
                buffer.push_back(data.at(i + j));
            }
            errors = (*this.*decoder)(buffer);
            buffer.resize(buffer.size() - getnRoots());
            block_errors.push_back(errors);
            interleaver.insert(interleaver.end(),buffer.begin(),buffer.end());
            buffer.clear();
        }

        //De-interleaving
        std::vector<unsigned char> deinterleaver;
        for(int j = 0; j < interleaver.size()/getDepth(); j++)
        {
            for(int i = 0; i < interleaver.size(); i += interleaver.size()/getDepth())
            {
                deinterleaver.push_back(interleaver.at(i + j));
            }
        }
        data = deinterleaver;

        //Fail or success
        bool success = true;
        for(int j=0; j < block_errors.size(); j++)
        {
            success  = (success && (block_errors.at(j) >= 0));
            wer += (block_errors.at(j) < 0) ? 1 : 0;
        }
        return success ? 0 : -1;
    }
}

//Encoder and decoder
void
reedSolomon::encodeRS223(std::vector<unsigned char> &data)
{
    if (d_basis)
    {
        basis2Conv(data);
        rs223.encode(data);
        conv2Basis(data);
    }
    else
    {
        rs223.encode(data);
    }
}

int reedSolomon::decodeRS223(std::vector<unsigned char> &data)
{
    int errors;
    if (d_basis)
    {
        basis2Conv(data);
        errors = rs223.decode(data);
        conv2Basis(data);
    }
    else
    {
        errors = rs223.decode(data);
    }
    return errors;
}

void
reedSolomon::encodeRS239(std::vector<unsigned char> &data)
{
    if (d_basis)
    {
        basis2Conv(data);
        rs239.encode(data);
        conv2Basis(data);
    }
    else
    {
        rs239.encode(data);
    }
}

int
reedSolomon::decodeRS239(std::vector<unsigned char> &data)
{
    int errors;
    if (d_basis)
    {
        basis2Conv(data);
        errors = rs239.decode(data);
        conv2Basis(data);
    }
    else
    {
        errors = rs239.decode(data);
    }
    return errors;
}

void
reedSolomon::basis2Conv(std::vector<unsigned char> &data)
{
    for(int i = 0; i < data.size(); i++)
        data.at(i) = Tal1tab[data.at(i)];
}

void
reedSolomon::conv2Basis(std::vector<unsigned char> &data)
{
    for(int j =0; j< data.size(); j++)
        data.at(j) = Taltab[data.at(j)];
}

//Setters
void
reedSolomon::setRSType(int rsType)
{
    d_rsType = rsType;
}

void
reedSolomon::setDepth(int depth)
{
    d_depth = depth;
}

void
reedSolomon::setBasis(int basis)
{
    d_basis = (basis >= 1) ? true : false;
}

//Getters
int
reedSolomon::getRSType() const
{
    return d_rsType;
}
int
reedSolomon::getDepth() const
{
    return d_depth;
}
bool
reedSolomon::getBasis() const
{
    return d_basis;
}
string
reedSolomon::getRSName() const
{
    return d_rsName;
}

int
reedSolomon::getnRoots() const
{
    return d_nRoots;
}
