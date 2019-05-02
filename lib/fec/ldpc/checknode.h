#ifndef CHECKNODE_H
#define CHECKNODE_H
#include <vector>
#include <algorithm>
#include <cmath>
class checkNode
{
private:
    std::vector<int> d_varNodeIndicies;
    std::vector<double> d_Lq;
    std::vector<double> d_Lr;
    double calculateLr(std::vector<double> lq, int index);
public:
    checkNode(std::vector<int> indicies);
    void updateLr();
    void updateLq(double newLq);
    double getLr(int index);
    std::vector<int> getVarNodes(){return d_varNodeIndicies;}
    std::vector<double> getLq(){return d_Lq;}
    std::vector<double> getLr(){return d_Lr;}
};

#endif // CHECKNODE_H
