/**
    zipReader.cpp
    Purpose: parser for files based on zip format

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"zipReader.hpp"

namespace   zipReader {
	void	readCentralDirectory(struct centralDirectory *dest, std::ifstream& file, const size_t offset) {
		file.seekg(offset, std::ios::beg);
		file.read(reinterpret_cast<char *>(&dest->headerSignature),			4);
		file.read(reinterpret_cast<char *>(&dest->versionMadeBy),			2);
		file.read(reinterpret_cast<char *>(&dest->versionNeededToExtract),	2);
		file.read(reinterpret_cast<char *>(&dest->bitFlag),					2);
		file.read(reinterpret_cast<char *>(&dest->compressionMethod),		2);
		file.read(reinterpret_cast<char *>(&dest->lastModFileTime),			2);
		file.read(reinterpret_cast<char *>(&dest->lastModFileDate),			2);
		file.read(reinterpret_cast<char *>(&dest->crc32),					4);
		file.read(reinterpret_cast<char *>(&dest->compressedSize),			4);
		file.read(reinterpret_cast<char *>(&dest->uncompressedSize),		4);
		file.read(reinterpret_cast<char *>(&dest->fileNameLength),			2);
		file.read(reinterpret_cast<char *>(&dest->extraFieldLength),		2);
		file.read(reinterpret_cast<char *>(&dest->fileCommentLength),		2);
		file.read(reinterpret_cast<char *>(&dest->diskNumberStart),			2);
		file.read(reinterpret_cast<char *>(&dest->internalFileAttributes),	2);
		file.read(reinterpret_cast<char *>(&dest->externalFileAttributes),	4);
		file.read(reinterpret_cast<char *>(&dest->relativeOffset),			4);
		if (dest->fileNameLength > 0) {
			dest->filename = new char[dest->fileNameLength + 1];
			file.read(dest->filename, dest->fileNameLength);
			dest->filename[dest->fileNameLength] = '\0';
		}
		if (dest->extraFieldLength > 0) {
			dest->extraField = new char[dest->extraFieldLength + 1];
			file.read(dest->extraField, dest->extraFieldLength);
			dest->extraField[dest->extraFieldLength] = '\0';
		}
		if (dest->fileCommentLength > 0) {
			dest->fileComment = new char[dest->fileCommentLength + 1];
			file.read(dest->fileComment, dest->fileCommentLength);
			dest->fileComment[dest->fileCommentLength] = '\0';
		}
		dest->isEncrypted = dest->strongEncryption = false;
		if (dest->bitFlag & 1)
			dest->isEncrypted = true;
		if (dest->bitFlag & (1 << 6))
			dest->strongEncryption = true;

		std::cout << "@@@@@@ CENTRAL DIRECTORY @@@@@@" << std::endl;
		std::cout << "starts at position: " << offset << std::endl;
		std::cout << "header signature: " << reinterpret_cast<unsigned int*>(dest->headerSignature)<<std::endl;
		std::cout << "version made by: " << dest->versionMadeBy <<std::endl;
		std::cout << "version needed to extract: " << dest->versionNeededToExtract <<std::endl;
		std::cout << "bit flag: " << dest->bitFlag <<std::endl;
		std::cout << "compression method: " << dest->compressionMethod <<std::endl;
		std::cout << "lastModFileTime: " << dest->lastModFileTime <<std::endl;
		std::cout << "lastModFileDate: " << dest->lastModFileDate <<std::endl;
		std::cout << "crc32: " << reinterpret_cast<unsigned int*>(dest->crc32) <<std::endl;
		std::cout << "compressedSize: " << reinterpret_cast<unsigned int*>(dest->compressedSize) <<std::endl;
		std::cout << "uncompressedSize: " << reinterpret_cast<unsigned int*>(dest->uncompressedSize) <<std::endl;
		std::cout << "diskNumberStart: " << dest->diskNumberStart <<std::endl;
		std::cout << "internalFileAttributes: " << dest->internalFileAttributes <<std::endl;
		std::cout << "externalFileAttributes: " << reinterpret_cast<unsigned int*>(dest->externalFileAttributes) <<std::endl;
		std::cout << "relativeOffset: " << reinterpret_cast<unsigned int*>(dest->relativeOffset) <<std::endl;
		if (dest->fileNameLength > 0)
			std::cout << "fileName: " << dest->filename <<std::endl;
		if (dest->extraFieldLength > 0)
			std::cout << "extraField: " << dest->extraField <<std::endl;
		if (dest->fileComment > 0)
			std::cout << "fileComment: " << dest->fileComment <<std::endl;
		std::cout << "encrypted:" << dest->isEncrypted <<std::endl;
		std::cout << "strong encryption:" << dest->strongEncryption <<std::endl;
	}
}