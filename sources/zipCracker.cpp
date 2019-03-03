/**
    zipCracker.cpp
    Purpose: brute force encrypted zip files

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"zipCracker.hpp"

zipCracker::zipCracker(const std::string& filename) : 
	_filename(filename),
	_file(filename.c_str(), std::ios::in | std::ios::binary),
	_eocd_offset(0),
	_cd({}),
	_eocd({})
{
	std::cout << "new zipcracker with file:"<< filename<<std::endl;	
}

zipCracker::~zipCracker() {}

bool		zipCracker::_checkHeader(void) {	
	uint32_t	*header_signature = new uint32_t;
	uint32_t	zip_signature = 0x04034b50;
	bool		found;

	_file.read(reinterpret_cast<char*>(header_signature), 4);
	found = *header_signature == zip_signature;
	delete header_signature;
	return found;
}

bool			zipCracker::_getEndOfCentralDirectoryOffset(void) {
	uint32_t	*signature = new uint32_t;
	uint32_t	eocd_signature = 0x06054b50;
	bool		found;
	size_t	i;

	_file.seekg(-4, std::ios::end);
	i = _file.tellg();
	for (;; --i) {
		_file.seekg(i, std::ios::beg);
		_file.read(reinterpret_cast<char*>(signature), 4);
		if (*signature == eocd_signature) {
				found = true;
				_eocd_offset = i;
				break;
		}
	}
	delete signature;
	return found;
}

void		zipCracker::_initStructures(void) {
	zipReader::readEndOfCentralDirectory(&_eocd, _file, _eocd_offset);
	centralDirectory toto;
	zipReader::readCentralDirectory(&toto, _file, _eocd.centralDirectoryOffset);
	//std::cout << _eocd.numberOfEntries<< ";at " <<std::hex << _eocd.centralDirectoryOffset << std::endl;
}

bool		zipCracker::isValid(void) {
	if (!_file.is_open()) {
		std::cerr << "I can't open this file -> " << _filename <<std::endl;
		return false;
	}
	if (!_checkHeader() ) {
		std::cerr << "Bad ZIP file (wrong headers)." << std::endl;
		return false;
	}
	_file.seekg(0, std::ios::end);
	size_t size = _file.tellg();
	if ( size < 22 ) {
		std::cerr << "The file size is " << size << " bytes, but the minimum size is 22 bytes." << std::endl;
		return false;
	}
	if (!_getEndOfCentralDirectoryOffset()) {
		std::cerr << "Unable to find Central Directory." << std::endl;
		return false;
	}
	std::cout << "Found Central directory at: 0x"<< std::hex <<_eocd_offset<< std::endl;
	_initStructures();
	return true;
}