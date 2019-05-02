// Example program
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
unsigned char swap(unsigned char oct);
unsigned char swapIQ(unsigned char in);
int main()
{
    unsigned h;
    //h =swap(0x00);
    unsigned char test = 0x22;
    unsigned int out = swapIQ(test);
    std::cout << "swapped byte : " << std::hex << out << std::endl;
    unsigned char IQSWAP[256];

    std::cout << "SWAP fUNCTION" << std::endl;
    for(unsigned int i = 0; i < 256; i++)
    {
        IQSWAP[i] = swapIQ(i&0xFF);
        std::cout << "0x" << std::hex << (swapIQ(i&0xFF) & 0xFF)  << " ";
        if (i % 15 == 0)
        {
            std::cout << std::dec << std::endl;
        }

    }
    std::cout << "SWAPPED ARRAY" << std::endl;
    for(unsigned int j = 0; j < 256; j++)
    {
        std::cout << "0x" << std::hex << ((IQSWAP[j])&0xFF) << " ";
        if (j % 15 == 0)
        {
            std::cout << std::dec << std::endl;
        }
        //std::cout << "Swapped 1 : " << std::hex << IQSWAP[j] << std::endl;
    }
    return 0;
}

unsigned char swap(unsigned char oct)
{       
    switch(oct)
    {
    case 0x0:
        oct = 0x0;
        break;
    case 0x1:
        oct = 0x2;
        break;
    case 0x2:
        oct = 0x1;
        break;
    case 0x3:
        oct = 0x3;
        break;
    case 0x4:
        oct = 0x8;
        break;
    case 0x5:
        oct = 0xA;
        break;
    case 0x6:
        oct = 0x9;
        break;
    case 0x7:
        oct = 0xB;
        break;
    case 0x8:
        oct = 0x4;
        break;
    case 0x9:
        oct = 0x6;
        break;
    case 0xA:
        oct = 0x5;
        break;
    case 0xB:
        oct = 0x7;
        break;
    case 0xC:
        oct = 0xC;
        break;
    case 0xD:
        oct = 0xE;
        break;
    case 0xE:
        oct = 0xD;
        break;
    case 0xF:
        oct = 0xF;
        break;
    }
   return oct;
}

unsigned char swapIQ(unsigned char in)
{
    unsigned  char oct1 = (in & 0x0F);
    unsigned  char oct0 = ((in >> 4) & 0x0F);
    unsigned  char out;

    /* std::cout << "Oct 0 : " << std::hex << oct0 << std::endl;
    std::cout << "Oct 1 : " << std::hex << oct1 << std::endl;

    std::cout << "Swapped Oct 0 : " << std::hex << swap(oct0) << std::endl;
    std::cout << "Swapped Oct 1 : " << std::hex << swap(oct1) << std::endl;*/
    out = (swap(oct0) << 4);
    out |= swap(oct1);
    return out;
}
