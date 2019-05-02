#include "checknode.h"

checkNode::checkNode(std::vector<int> indicies)
{
    /*for(unsigned int i = 0; i < indicies.size(); i++)
    {
        if (indicies[i] == 1)
            d_varNodeIndicies.push_back(i);
    }*/
    for(unsigned int i = 0; i < indicies.size(); i++)
     {

             d_varNodeIndicies.push_back(indicies[i] - 1);
     }
}

double
checkNode::calculateLr(std::vector<double> lq, int index)
{
    double prod = 1.0;
    for(unsigned int i = 0; i < lq.size(); i++)
    {
        if (i != index)
            prod *= tanh(lq[i]/2);
    }
    return 2*atanh(prod);
}

void
checkNode::updateLr()
{
    d_Lr.clear();
    for(unsigned int i = 0; i < d_Lq.size(); i++)
    {
        d_Lr.push_back(calculateLr(d_Lq,i));
    }
    d_Lq.clear();
    return;
}

void
checkNode::updateLq(double newLq)
{
    d_Lq.push_back(newLq);
    return;
}

double
checkNode::getLr(int index)
{
    return d_Lr[index];
}
