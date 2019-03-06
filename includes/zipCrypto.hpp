/**
    zipCrypto.hpp
    Purpose:  implementation of the basic PKWARE encryption

    @author Louis JURCZYK
    @version 1.0 06/03/19
*/

#ifndef         _ZIPCRYPTO_HPP_
#define         _ZIPCRYPTO_HPP_

#include        <iostream>
extern uint32_t         keysCRC[3];
extern const uint32_t   pcrc_32_tab[8][256];

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
/*#define POLY 0x82f63b78*/

/*#define POLY 0xedb88320 

uint32_t    crc32c(uint32_t crc, const char *buf, size_t len){
    int k;

    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
    }
    return ~crc;
}
*/


unsigned char   decryptByte();
void            updateKeys(const int& c);
void            initKeys(char *password);


#endif      /*_ZIPCRYPTO_HPP_*/