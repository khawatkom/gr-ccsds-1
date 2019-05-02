#ifndef DEBUG
#define DEBUG
#include <iostream>
#include <string>
//#include <itpp/itcomm.h>
using namespace std;
//using namespace itpp;
#include "ldpc.h"
string path = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/LDPC/ldpc_2/";


void printRow(std::vector<int> row)
{
    for(unsigned int i = 0; i < row.size(); i++)
    {
        std::cout << row[i] << " " ;
    }
    std::cout << std::endl;
}
void printParity(std::vector<std::vector<int> > parity)
{
    int R = parity.size();
    int C = parity.at(0).size();
    std::cout << "Parity matrix size : " << R << " X " << C << std::endl;
    for(int r = 0 ; r < R; r++)
    {
        printRow(parity[r]);
    }
}



template <class T>
std::vector<T> getCol(std::vector< std::vector<T> > mat2D,int index)
{
    std::vector<T> col;
    for(unsigned int i = 0; i < mat2D.size(); i++)
    {
        col.push_back(mat2D[i][index]);
    }
    return col;
}

template <class T>
std::vector<T> getRow(std::vector< std::vector<T> > mat2D, int index)
{
    return mat2D[index];
}

void printVarNodeLq(variableNode node)
{
    std::vector<double> lq = node.getLq();
    for(unsigned int i = 0; i < lq.size(); i++)
    {
        std::cout << "Lq(" << i << ") = " << lq[i] << " ";
    }
    std::cout << std::endl;
}

void printChkNodeLq(checkNode node)
{
    std::vector<double> lq = node.getLq();
    for(unsigned int i = 0; i < lq.size(); i++)
    {
        std::cout << "Lq(" << i << ") = " << lq[i] << " ";
    }
    std::cout << std::endl;
}

void printChkNodeLr(checkNode node)
{
    std::vector<double> lr = node.getLr();
    for(unsigned int i = 0; i < lr.size(); i++)
    {
        std::cout << "Lr(" << i << ") = " << lr[i] << " ";
    }
    std::cout << std::endl;
}

void printVarNodeLogAPP(std::vector<variableNode> v)
{
    //std::cout << "Size of logapp : " << v.size() << std::endl;
    std::cout << "[ ";
    for(unsigned int i = 0; i < v.size(); i++)
        std::cout << v[i].getlogAPP() << " ";
    std::cout << "]" << std::endl;
    std::cout << "[ ";
    for(unsigned int i = 0; i < v.size(); i++)
        std::cout << ((v[i].getlogAPP() > 0) ? 0 : 1) << " ";
    std::cout << "]" << std::endl;
}
std::vector< std::vector<int> > readAlist(string filename,
                                          std::vector< std::vector<int> > &m_list,std::vector< std::vector<int> > &n_list)
{
    filename = path + filename;
    std::cout <<"File path : " << filename << std::endl;
    alist myAlist(filename.c_str());
    std::cout << "Alist file read" << std::endl;
    std::vector< std::vector<char> > H = myAlist.get_matrix();
    std::cout << "alist matrix read" << std::endl;
    std::vector< std::vector<int> > H_int;
    int R = H.size();
    int C = H[0].size();

    std::cout << "Size of R : " << R << std::endl;
    std::cout << "Size of C : " << C << std::endl;
    for(int r = 0; r < R; r++)
    {
        std::vector<int> row;
        for(int c = 0; c < C; c++)
        {
           //std::cout << int(H[r][c]);

           row.push_back(int(H[r][c]));
        }
        H_int.push_back(row);
        row.clear();
    }
    m_list = myAlist.get_mlist();
    n_list = myAlist.get_nlist();
    /*
    for (int i = 0; i < m_list.size(); i++)
    {
        for(int j = 0; j < m_list[0].size(); j++)
        {
            std::cout << m_list[i][j] << " ";
        }
        std::cout << std::endl;
    }*/

    return H_int;
}

void ldpcTDD()
{
    const string filename = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/LDPC/ldpc_2/myParity";
    ldpc myCode(filename);
    double coderate = myCode.getCodeRate();
    std::vector<double> y1{+0.2, +0.2, -0.9, +0.6, +0.5, -1.1, -0.4, -1.2};
    std::vector<double> logapp = myCode.decode(y1,7,0.5);
    for(unsigned int i = 0; i < logapp.size(); i++)
        std::cout << logapp[i] << " ";
    std::cout << std::endl;
}

/*
 void saveAlist(string filename , std::vector<std::vector<int> > mat)
 {
     int R = mat.size();
     int C = mat[0].size();
     LDPC_Parity itppParity = LDPC_Parity(R,C);
     for(int r = 0; r < R; r++)
     {
         for(int c = 0; c < C; c++)
            itppParity.set(r,c,mat[r][c]);
     }
     //Save alist file
     //string path = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/LDPC/ldpc_2/" + filename;
     itppParity.save_alist(path + filename);
 }*/


#endif // DEBUG

