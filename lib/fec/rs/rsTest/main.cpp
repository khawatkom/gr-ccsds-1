#include <iostream>
#include <string>
#include <string.h>
#include <getopt.h>
#include "../rs"
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;

typedef std::vector<unsigned char> bytes;

//The name of this program
string program_name;


//Printing usage information
void print_usage(int exit_code)
{
  cout << "Usage: " << program_name << "options [ inputfile ... ]"      << std::endl;
  cout << "  -h --help              Display this usaga information."    << std::endl;
  cout << "  -i --input filename    Read hex data from a file."         << std::endl;
  cout << "  -n --number of packets Number of RS codewords"             << std::endl;
  cout << "  -v --verbose           Print verbose messages."            << std::endl;
  cout << " Example ./test -f rs_qbee_2.bin -n 25"                      << std::endl;
  exit(exit_code);
}


//Reading bytes and save them in std::vector<unsigned char>
bytes readFile(string fileName)
{
    ifstream in;
    in.open(fileName.c_str(),ios::binary);
    if (in.fail())
    {
        //error
    }
    bytes inFile;
    while (!in.eof())
    {
        unsigned char byte;
        in >> byte;
        if (in.fail())
        {
            //error
            break;
        }
        inFile.push_back(byte);
    }
    in.close();
    return inFile;
}

//Hexdump function
void hexdump(void *ptr, int buflen) {
        unsigned char *buf = (unsigned char*) ptr;
        int i, j;
        for (i = 0; i < buflen; i += 16) {
                printf("%06x: ", i);
                for (j = 0; j < 16; j++)
                        if (i + j < buflen)
                                printf("%02x ", buf[i + j]);
                        else
                                printf("   ");
                printf(" ");
                for (j = 0; j < 16; j++)
                        if (i + j < buflen)
                                printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
                printf("\n");
        }
}

//Hexdump function for a vector
void vectorhexdump(bytes data) {
        hexdump(reinterpret_cast<char*>(data.data()), data.size());
        std::cout << std::endl << std::endl;
}


int main(int argc, char* argv[])
{
    //Reading options
    //A string listing valid short options letters
    const char* const short_options = "hf:n:v";
    //An array describing valid long options
    const struct option long_options[] = {
      { "help",   0, NULL, 'h' },
      { "file",  1, NULL, 'f' },
      { "num",  1, NULL, 'n' },
      { "verbose",0, NULL, 'v' },
      {  NULL    ,0, NULL,  0}
       };
    /*Remember the name of the program, to incorporate in message. The name is stored
      in argv[0]*/
    program_name = argv[0];

    //Reading options
    string inputFile;
    int verbose;
    int next_option;
    int numPackets = 25;
    do {
      next_option = getopt_long(argc,argv, short_options, long_options, NULL);

      switch(next_option)
        {
        case 'h': // -h or --help
          /* User has requested usage information. Print it to standard
            output, and exit with exit code zero*/
          print_usage(0);
          break;

        case 'f': // -o or --output
          /* This option takes an argument, the name of the output */
          inputFile = optarg;
          break;

      case 'n':
          numPackets = atoi(optarg);

        case 'v': // -v or --verbose
          verbose = 1;
          break;

        case '?': // The user has specified an invalid option
          /* Print usage information to standard error, and exit with
             exit code one (indication abnormal termination).*/
          print_usage(1);

        case -1: // Done with options
          break;

        default: //Something else : unexpected
          abort();
        }
    }while (next_option != -1);



    cout << "Input file name : " << inputFile << std::endl;
    bytes input  = readFile(inputFile);
    bytes buffer = input;

    //Hex-dumping to ensure frames are read correctly
    std::cout << "Input File" << std::endl;
    vectorhexdump(input);

    //Reading data from a file
    ezpwd::RS_CCSDS<255,223> rs;

    for(int i = 0; i < numPackets; i++)
    {
        rs.encode(input);
        //std::cout << "After reed solomon" << std::endl;
        //vectorhexdump(input);
        int error = rs.decode(input);
        std::cout << "Error Status : " << error << std::endl;
        input = buffer;
    }
    return 0;

}

