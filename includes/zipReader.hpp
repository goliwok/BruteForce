/**
    zipReader.hpp
    Purpose: parser for files based on zip format

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#ifndef         _ZIPREADER_HPP_
#define         _ZIPREADER_HPP_

#include        <iostream>
#include        <fstream>
#include        <vector>
#include        <map>

enum CompressionMethod {
    STORED      = 0,
    SHRUNK      = 1,
    IMPLODED    = 6,
    DEFLATED    = 8,
    DEFLATE64   = 9,
    PKWARE      = 10,
    BZIP2       = 12,
    LZMA        = 13,
    JPEG        = 96,
    WAVPACK     = 97,
    PPMD        = 98,
    AEX         = 99
};
inline std::ostream& operator<<(std::ostream& out, const CompressionMethod value){
    static std::map<CompressionMethod, std::string> strings;
    if (strings.size() == 0){
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(STORED);
        INSERT_ELEMENT(SHRUNK);     
        INSERT_ELEMENT(DEFLATED);
        INSERT_ELEMENT(DEFLATE64);
        INSERT_ELEMENT(PKWARE);
        INSERT_ELEMENT(BZIP2);
        INSERT_ELEMENT(LZMA);
        INSERT_ELEMENT(JPEG);
        INSERT_ELEMENT(WAVPACK);
        INSERT_ELEMENT(PPMD);
        INSERT_ELEMENT(AEX);         
#undef INSERT_ELEMENT
    }   

    return out << strings[value];
}
struct endOfCentralDirectory {
    uint32_t headerSignature;
    uint16_t numberDisk;
    uint16_t startOffset;
    uint16_t startDisk;              
    uint16_t numberOfEntries;
    uint32_t centralDirectorySize;
    uint32_t centralDirectoryOffset;
    uint16_t commentLength;

    char    *comment;
    
    ~endOfCentralDirectory() {
        if (commentLength > 0)
            delete[] comment;
    }
};

struct centralDirectory {
    uint32_t headerSignature;
    uint16_t versionMadeBy;
    uint16_t versionNeededToExtract;
    uint16_t bitFlag;
    uint16_t compressionMethod;
    uint16_t lastModFileTime;
    uint16_t lastModFileDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    uint16_t fileNameLength;
    uint16_t extraFieldLength;
    uint16_t fileCommentLength;
    uint16_t diskNumberStart;
    uint16_t internalFileAttributes;
    uint32_t externalFileAttributes;
    uint32_t relativeOffset;

    char    *filename;
    char    *extraField;
    char    *fileComment;

    bool    isEncrypted;
    bool    strongEncryption;

    ~centralDirectory() {
        if (fileNameLength >  0)
            delete[] filename;
        if (extraFieldLength > 0)
            delete[] extraField;
        if (fileCommentLength > 0)
            delete[] fileComment;
    }
};

struct encryptionHeader {
    uint32_t    key1;
    uint32_t    key2;
    uint32_t    key3;
    encryptionHeader() { key1 = key2 = key3 = 0; }
};

struct dataDescriptor {
    uint32_t headerSignature;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;

    dataDescriptor(){ headerSignature = crc32 = compressedSize = uncompressedSize = 0; }
};

struct localFileHeader {
    uint16_t checkByte;    
    uint32_t headerSignature;
    uint16_t versionNeededToExtract;
    uint16_t bitFlag;
    uint16_t compressionMethod;
    uint16_t lastModFileTime;
    uint16_t lastModFileDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    uint16_t fileNameLength;
    uint16_t extraFieldLength;
    uint32_t dataLength;
    uint32_t dataStartOffset;
    char    *filename;
    char    *extraField;
    char    *data;

    bool    isEncrypted;
    bool    strongEncryption;

    dataDescriptor      *descriptor;
    encryptionHeader    *eh;

    localFileHeader() {
        descriptor = new dataDescriptor;
        eh = new encryptionHeader;
        isEncrypted = strongEncryption = false;
        dataLength = 0;
    }
    ~localFileHeader() {
        delete descriptor;
        delete eh;
        if (fileNameLength >  0)
            delete[] filename;
        if (extraFieldLength > 0)
            delete[] extraField;
    }
};

namespace   zipReader {
    void    readLocalFileHeader(struct localFileHeader* dest, std::ifstream& file);
    void    readEncryptionHeader(struct localFileHeader* dest, std::ifstream& file);
    void    readDataDescriptor(struct localFileHeader* dest, std::ifstream& file);
    void    readCentralDirectory(struct centralDirectory* dest, std::ifstream& file);
    void    readEndOfCentralDirectory(struct endOfCentralDirectory *dest, std::ifstream& file);
}

#endif      /*_ZIPREADER_HPP_*/