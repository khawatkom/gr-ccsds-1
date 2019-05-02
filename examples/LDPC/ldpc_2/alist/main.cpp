#include <iostream>

using namespace std;
#include <vector>
#include <algorithm>
#include <itpp/itcomm.h>


#define N 8176
#define K 7154
#define P (N - K)
using namespace itpp;
using namespace std;
void
leftrotate(string &s, int d)
{
    reverse(s.begin(), s.begin()+d);
    reverse(s.begin()+d, s.end());
    reverse(s.begin(), s.end());
}
// In-place rotates s towards right by d
void
rightrotate(string &s, int d)
{
    leftrotate(s, s.length()-d);
}
void C2_Parity()
{
    std::vector<std::string> A1;
    std::vector<std::string> A2;
    std::vector< std::vector<int> > A1_indicies{{0, 176},  {12, 239},  {0, 352},   {24, 431}, {0, 392}, {151, 409}, {0, 351}, {9, 359}, {0, 307}, {53, 329}, {0, 207}, {18, 281}, {0, 399}, {202, 457}, {0, 247}, {36, 261}};
    std::vector< std::vector<int> > A2_indicies{{99, 471}, {130, 473}, {198, 435}, {260, 478}, {215, 420}, {282, 481}, {48, 396}, {193, 445}, {273, 430}, {302, 451}, {96, 379}, {191, 386}, {244, 467}, {364, 470}, {51, 382}, {192, 414}};

    std::cout <<"size of A1 Indicies: " << A1_indicies.size() << std::endl;
    std::cout <<"size of A2 Indicies: " << A2_indicies.size() << std::endl;

    //Initialize strings
    const int size = 511;
    std::string zeros = std::string(size,'0');
    //std::cout << zeros;
    for(int i = 0; i < 16; i++)
    {
        A1.push_back(zeros);A1[i][A1_indicies[i][0]] = '1';A1[i][A1_indicies[i][1]] = '1';
        A2.push_back(zeros);A2[i][A2_indicies[i][0]] = '1';A2[i][A2_indicies[i][1]] = '1';
    }
    std::cout << "Size of A1 : " << A1.size() << std::endl;
    std::cout << "Size of A2 : " << A1.size() << std::endl;

    std::vector<std::string> hString;


    //Upper matrices
    for(int j = 0; j < 511;j++)
    {

        std::string a;
        for(int i = 0; i < 16; i++)
            a += A1[i];
        std::cout << a ;
        hString.push_back(a);
        for(int r = 0; r < 16; r++)
            rightrotate(A1[r],1);
    }

    //Lower matrices
    for(int j = 0; j < 511;j++)
    {

        std::string a;
        for(int i = 0; i < 16; i++)
            a += A2[i];
        std::cout << a ;
        hString.push_back(a);
        for(int r = 0; r < 16; r++)
            rightrotate(A2[r],1);
    }




    int R = hString.size();
    int C = hString[0].length();
    LDPC_Parity itppParity = LDPC_Parity(R,C);
    for(int r = 0; r < R; r++)
    {
        for(int c = 0; c < C; c++)
           itppParity.set(r,c,(hString[r][c] == '1') ? 1 : 0);
    }
    //Save alist file
    string filename = "c2test.a";
    string path = "/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/examples/LDPC/ldpc_2/" + filename;
    itppParity.save_alist(path + filename);

    std::cout << "Size of hString : " << hString.size() << " x " << hString[0].length() << std::endl;

    //Generating parity matrix

    abort();
}
int main()
{
    cout << "Hello World!" << endl;
     C2_Parity();
    return 0;
}

