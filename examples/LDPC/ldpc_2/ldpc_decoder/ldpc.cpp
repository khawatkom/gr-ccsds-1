#include "ldpc.h"

ldpc::ldpc(string filename)
{
    //Reading alist file
    std::vector< std::vector<int> > m_list, n_list;
    readAlist(filename,m_list,n_list);
    codeRate = (n_list.size()*1.0 - m_list.size()*1.0)/(n_list.size()*1.0);

    //Initializing check nodes
    for(unsigned int i = 0; i < m_list.size(); i++)
        checkNodes.push_back(checkNode(m_list[i]));

    // Initializing variable nodes
    for(unsigned int i = 0; i < n_list.size(); i++)
    {
        varNodes.push_back(variableNode(n_list[i]));
    }

    // Deriving code properties
   numCheckNodes = m_list.size();
   numVarNodes   = n_list.size();
}

void
ldpc::readAlist(string filename, std::vector< std::vector<int> > &m_list,std::vector< std::vector<int> > &n_list)
{
    alist myAlist(filename.c_str());
    m_list = myAlist.get_mlist();
    n_list = myAlist.get_nlist();
}

std::vector<double>
ldpc::decode(std::vector<double> softBits, int iterations, double sigma)
{
    // ##############################################
    // Initializing variable nodes
    for(unsigned int i = 0; i < softBits.size(); i++)
        varNodes[i].setLx(2*softBits[i]/(sigma*sigma));

    // ###############################################
    for(int z = 0; z < iterations; z++)
    {
        // ##########################################
        // Pass Lq from variable nodes to check nodes
        for(unsigned int v = 0; v < varNodes.size();v++)
        {
            std::vector<int> indicies = varNodes[v].getCheckNodes();
            std::vector<double> lq    = varNodes[v].getLq();
            assert(lq.size() == indicies.size());
            for(unsigned int c = 0; c < indicies.size();c++)
                checkNodes[indicies[c]].updateLq(lq[c]);
        }

        // ##########################################
        // Updating Checknode Lrs
        for(unsigned int i = 0; i < checkNodes.size(); i++)
        {
            checkNodes[i].updateLr();
        }


        // ##########################################
        for(unsigned int c = 0; c < checkNodes.size();c++)
        {
            std::vector<int> indicies = checkNodes[c].getVarNodes();
            std::vector<double> lr    = checkNodes[c].getLr();
            assert(lr.size() == indicies.size());
            for(unsigned int r = 0; r < indicies.size();r++)
                varNodes[indicies[r]].updateLr(lr[r]);
        }

        // #########################################
        // Update Lqs
        for(unsigned int i = 0; i < varNodes.size(); i++)
            varNodes[i].updateLq();

        // ##########################################
        // Updating logAPP
        for(unsigned int v = 0; v < varNodes.size(); v++)
            varNodes[v].updatelogAPP();
    }
    //std::cout << "Size of varNodes : " << varNodes.size() << std::endl;
    std::vector<double> logAPP;
    for(unsigned int i = 0; i <(varNodes.size() - getnumCheckNodes()); i++)
    {
        logAPP.push_back(varNodes[i].getlogAPP());
    }
    //std::cout << "Size of logAPP vector : " << logAPP.size() << std::endl;
    return logAPP;
}
