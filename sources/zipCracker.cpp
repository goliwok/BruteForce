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

zipCracker::~zipCracker() {
	for (int i = 0; i < _cd.size(); i++)
		delete _cd[i];
}

bool		zipCracker::_checkHeader(void) {	
	uint32_t	*header_signature = new uint32_t;
	uint32_t	zip_signature = 0x04034b50;
	bool		found;

	_file.read(reinterpret_cast<char*>(header_signature), 4);
	found = *header_signature == zip_signature;
	delete header_signature;
	return found;
}

bool		zipCracker::_getEndOfCentralDirectoryOffset(void) {
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
				_eocd_offset = i;
				found_end = true;
				i -= 2;
		} else if (*signature == start_signature) {
				found = true;
				break;
		}
	}
	_file.seekg(0, std::ios::end);
	std::cout << "file size after getend:" << _file.tellg() << std::endl;
	delete signature;
	return found;
}
void		zipCracker::_initStructures(void) {
	zipReader::readEndOfCentralDirectory(&_eocd, _file, _eocd_offset);
	std::vector<centralDirectory *> v;

	centralDirectory *toto = new centralDirectory;
	zipReader::readCentralDirectory(toto, _file, _eocd.centralDirectoryOffset);
	_cd.push_back(toto);
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