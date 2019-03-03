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
	_start(0),
	_end(0),
	_cd({}) 
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

bool		zipCracker::_getCentralDirectory(void) {
	uint32_t	*signature = new uint32_t;
	bool		found_end = false, found = false;;
	uint32_t	start_signature = 0x02014b50;
	uint32_t	end_signature = 0x06054b50;
	size_t	i;

	_file.seekg(-4, std::ios::end);
	i = _file.tellg();
	for (; i > 3; --i) {
		_file.seekg(i, std::ios::beg);
		_file.read(reinterpret_cast<char*>(signature), 4);
		if (!found_end && *signature == end_signature) {
				_end = i;
				found_end = true;
				i -= 2;
		} else if (*signature == start_signature) {
				_start = i;
				found = true;
				break;
		}
	}
	delete signature;
	return found;
}

void		zipCracker::_initStructures(void) {
	zipReader::readCentralDirectory(&_cd, _file, _start);
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
	if (!_getCentralDirectory()) {
		std::cerr << "Unable to find Central Directory." << std::endl;
		return false;
	}
	std::cout << "Central directory: "<< _start << ";" << _end << std::endl;
	_initStructures();
	return true;
}