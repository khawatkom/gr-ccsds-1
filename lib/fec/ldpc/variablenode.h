#ifndef VARIABLENODE_H
#define VARIABLENODE_H
#include <vector>
#include <algorithm>
#include <iostream>

class variableNode
{
private:
    std::vector<int>  d_checkNodeIndicies;
    std::vector<double> d_Lr;
    std::vector<double> d_Lq;
    double lx;
    double logAPP;
protected:
    double calculateLq(std::vector<double> lr, unsigned int index);
    double sum(std::vector<double> vec);
public:
    variableNode(std::vector<int> indicies);
    std::vector<int> getCheckNodes(){return d_checkNodeIndicies;}
    std::vector<double> getLq(){return d_Lq;}
    std::vector<double> getLr(){return d_Lr;}
    void setLx(double newLx);
    void updatelogAPP();
    double getlogAPP(){return logAPP;}
    void updateLq();
    void updateLr(double newLr);


};

#endif // VARIABLENODE_H
