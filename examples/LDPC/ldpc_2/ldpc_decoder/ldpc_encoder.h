#ifndef LDPC_ENCODER
#define LDPC_ENCODER
#include <vector>
#include <algorithm>
//#include <itpp/itcomm.h>


#define N 8176
#define K 7154
#define P (N - K)
//using namespace itpp;
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

std::vector<std::string> splitString(const std::string& str, std::size_t len)
{
    if (len >= str.size())
        return { str };
    auto it = str.begin();
    auto end = it + len;
    std::vector<std::string> strings;
    while (end != str.end())
    {
        strings.emplace_back(it, end);
        ++end;
        ++it;
    }
    // have to do this to get the last string since end == str.end()
    strings.emplace_back(it, end);
    return strings;
}

std::vector < std::vector<unsigned char> >
generatorMatrix()
{
    //Reading first circulants rows of generator matrix
    std::ifstream infile;
    infile.open("/home/mbkitine/Dropbox/Lulea/GRC/DeepSpace/gr-ccsds/lib/fec/ldpc/gmini/C2.txt");
    if (!infile) {
        cerr << "Unable to open file circulants.txt";
        exit(1);   // call system to stop
    }
    std::string line;
    std::vector<std::string> circulants;
    while (std::getline(infile, line))
    {
        circulants.push_back(line);
    }
    //std::cout << "Size of circulant before circulation : " << circulants.size() << std::endl;
    const int numCirculants   = 14;
    const int numCirculations = 511;
    const int lenCirculants   = 511*2;

    std::vector<std::string> genMtx;
    std::vector < std::vector<unsigned char> > G;
    for(int i = 0; i < numCirculants; i++)
    {
        std::string curCirculant = circulants.at(i);
        std::string b1 = curCirculant.substr(0,511);
        std::string b2 = curCirculant.substr(511,511);
//        std::cout << "Length of b  : " << curCirculant.length() << std::endl;
//        std::cout << "Length of b1 : " << b1.length() << std::endl;
//        std::cout << "Length of b2 : " << b2.length() << std::endl;
        //std::cout << b1 << b2 << std::endl;
        //std::string b = b1 + b2;
        //std::cout << b << std::endl;
        for(int j = 0; j < numCirculations; j++)
        {
            std::string b = b1 + b2;
            //genMtx.push_back(curCirculant);
            genMtx.push_back(b);
            rightrotate(b1,1);
            rightrotate(b2,1);

//            if((j < 3) && (i < 1))
//                std::cout << b1 << std::endl;
        }
    }
    //std::cout << "Size of circulant after circulation : " << genMtx.size() << std::endl;

    std::vector<unsigned char> row;
    for(int k = 0; k < numCirculants*numCirculations; k++)
    {

        for(int i = 0; i < lenCirculants; i++)
        {

            row.push_back((genMtx.at(k)[i] == '1') ? 0x01 : 0x00);

        }
        G.push_back(row);
        row.clear();
    }

    std::cout << "Generator matrix initialized" << std::endl;
    std::cout << "N : " << N << std::endl;
    std::cout << "K : " << K << std::endl;
    std::cout << "P : " << P << std::endl;
    return G;
}
std::vector<unsigned char> ldpc_encode(std::vector<unsigned char> input, std::vector < std::vector<unsigned char> > G)
{
//Initializing parity bits to 0x00
      std::vector<unsigned char> parity(P, 0x00);

      //Generating parity bits
      for(int i = 0; i < K; i++)
      {
          if(input.at(i))//input.at(i)
          {
              std::transform(parity.begin(), parity.end(), G.at(i).begin(),
                  parity.begin(), std::bit_xor<unsigned char>());
          }
      }

      //Appending parity bits to the original input
      input.insert(input.end(), parity.begin(), parity.end());
      return input;
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


    //LDPC_Parity itppParity;
    /*
    int R = hString.size();
    int C = hString[0].length();
    LDPC_Parity itppParity = LDPC_Parity(R,C);
    for(int r = 0; r < R; r++)
    {
        for(int c = 0; c < C; c++)
           itppParity.set(r,c,(hString[r][c] == '1') ? 1 : 0);
    }*/

    std::cout << "Size of hString : " << hString.size() << " x " << hString[0].length() << std::endl;

    //Generating parity matrix

    abort();
}

#endif // LDPC_ENCODER

