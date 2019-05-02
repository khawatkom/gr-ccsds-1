#include "variablenode.h"

variableNode::variableNode(std::vector<int> indicies)
{
    /*for(unsigned int i = 0; i < indicies.size(); i++)
    {
        if (indicies[i] == 1)
            d_checkNodeIndicies.push_back(i);
    }*/

   for(unsigned int i = 0; i < indicies.size(); i++)
    {

            d_checkNodeIndicies.push_back(indicies[i] - 1);
    }
}

double
variableNode::calculateLq(std::vector<double> lr, unsigned int index)
{
    double sum = 0.0;
    for(unsigned int i = 0; i < lr.size(); i++)
    {
        if (i != index)
            sum += lr[i];
    }
    return (sum + lx);
}

void
variableNode::updateLq()
{
    d_Lq.clear();
    for(unsigned int i = 0; i < d_Lr.size(); i++)
    {
        d_Lq.push_back(calculateLq(d_Lr,i));
    }

    return;
}

void
variableNode::updateLr(double newLr)
{
    d_Lr.push_back(newLr);
}

void
variableNode::setLx(double newLx)
{
    d_Lq.clear();
    lx = newLx;
    for(unsigned int i = 0; i < d_checkNodeIndicies.size(); i++)
    {
        d_Lq.push_back(lx);
    }
}

void
variableNode::updatelogAPP()
{
    logAPP = lx + sum(d_Lr);
    d_Lr.clear();
}
double
variableNode::sum(std::vector<double> vec)
{
    double sum = 0.0;
    for(unsigned int i = 0; i < vec.size(); i++)
        sum+= vec[i];
    return sum;
}

