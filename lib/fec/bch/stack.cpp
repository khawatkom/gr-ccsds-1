typedef unsigned char uint8_t;
typedef unsigned char crc;
#define WIDTH (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
crc crcTable[256];
#define POLYNOMIAL 0xC5
void
crcInit(void)
{
    crc remainder;


    /*
* Compute the remainder of each possible dividend.
*/
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        /*
* Start with the dividend followed by zeros.
*/
        remainder = dividend << (WIDTH - 8);

        /*
* Perform modulo-2 division, a bit at a time.
*/
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            /*
* Try to divide the current data bit.
*/
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
* Store the result into the table.
*/
        crcTable[dividend] = remainder;
    }

} /* crcInit() */


crc
crcFast(uint8_t const message[], int nBytes)
{
    uint8_t data;
    crc remainder = 0;


    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (remainder);

}   /* crcFast() */
