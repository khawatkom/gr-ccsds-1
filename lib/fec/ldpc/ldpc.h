#ifndef LDPC_H
#define LDPC_H
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include "checknode.h"
#include "variablenode.h"
#include "alist.h"
using namespace std;
class ldpc
{
private:
    std::vector<checkNode> checkNodes;
    std::vector<variableNode> varNodes;
    int numCheckNodes;
    int numVarNodes;
    float codeRate;
public:
    ldpc(string filename);
    void readAlist(string filename, std::vector< std::vector<int> > &m_list,std::vector< std::vector<int> > &n_list);
    std::vector<float> decode(std::vector<float> softBits, int iterations, float sigma);
    float getCodeRate() { return codeRate;}
    int getnumCheckNodes() { return numCheckNodes;}
    int getnumVarNodes() { return numVarNodes;}

};

#endif // LDPC_H
