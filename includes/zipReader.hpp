/**
    zipReader.hpp
    Purpose: parser for files based on zip format

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#ifndef         _ZIPREADER_HPP_
#define         _ZIPREADER_HPP_

#include		<iostream>
#include        <fstream>
#include        <vector>

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

struct localFileHeader {
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

    char    *filename;
    char    *extraField;
    char    *data;
};

namespace   zipReader {
    void    readCentralDirectory(struct centralDirectory* dest, std::ifstream& file);
    void    readLocalFileHeader(struct localFileHeader* dest, std::ifstream& file);
    void    readEndOfCentralDirectory(struct endOfCentralDirectory *dest, std::ifstream& file);
}

#endif      /*_ZIPREADER_HPP_*/