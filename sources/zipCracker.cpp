/**
    zipCracker.cpp
    Purpose: brute force encrypted zip files

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"zipCracker.hpp"
#include	"zipCrypto.hpp"

zipCracker::zipCracker()          
{
	_filename = "";
	_eocd_offset = 0;
	_cd = {};
	_eocd = {};
	_buffer	= new uint8_t[12];
	_tmp_buffer = NULL;	
	_light = NULL;
}                                          

zipCracker::~zipCracker() {
	_file.close();
	_clean();
	if (_tmp_buffer)
		delete[] _tmp_buffer;
	delete[] _buffer;
	if (_light)
		delete _light;
}

void	zipCracker::_clean() {
	if (_lfh.size() > 0)
		for (auto i: _lfh)
			delete i;
	if (_cd.size() > 0)
		for (auto i: _cd)
			delete i;
	_lfh.clear();
	_cd.clear();
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
	uint32_t	eocd_signature = 0x06054b50;
	size_t	i;

	_file.seekg(-4, std::ios::end);
	i = _file.tellg();
	for (; i > 0;) {
		_file.seekg(i, std::ios::beg);
		_file.read(reinterpret_cast<char*>(signature), 4);
		if (!found_end && *signature == eocd_signature) {
			_eocd_offset = i;
			found = true;
			break;
		}
		i -= 3;
	}
	delete signature;
	return found;
}

void				zipCracker::_aggressiveFindLFH(void) {
	uint32_t		lfh_signature = 0x04034B50;
	uint32_t		eocd_signature = 0x06054b50;
	uint32_t		cd_signature = 0x02014B50;
	uint32_t		*signature = new uint32_t;
	localFileHeader *lfh;
	_file.seekg(0, std::ios::beg);

	for (size_t i = _file.tellg(); i < _eocd_offset ; i++){
		_file.seekg(i, std::ios::beg);
		_file.read(reinterpret_cast<char*>(signature), 4);
		if (*signature == lfh_signature){
			lfh = new localFileHeader;
			zipReader::readLocalFileHeader(lfh, _file);
			_lfh.push_back(lfh);
		} else if (*signature == cd_signature || *signature == eocd_signature)
			break;
	}
	delete signature;
}

void		zipCracker::_initStructures(void) {
	_file.seekg(_eocd_offset, std::ios::beg);
	zipReader::readEndOfCentralDirectory(&_eocd, _file);
	if (_eocd_offset == 0)
		std::cout << "File contains ONLY  end of central directory (empty file)" << std::endl;
	centralDirectory *cd;
	_file.seekg(_eocd.centralDirectoryOffset, std::ios::beg);
	for (int i = 0; i <= _eocd.numberOfEntries - 1; i++) {
		cd = new centralDirectory;
		zipReader::readCentralDirectory(cd, _file);
		_cd.push_back(cd);
	}
	_aggressiveFindLFH();
}

bool			zipCracker::selectSmallestFile(void) {
	uint32_t	smallestSize = 0;
	size_t		smallestLFH;

	for (size_t i = 0; i < _lfh.size(); i++) {
		std::cout << "file:"<< _lfh[i]->filename << " length: " << _lfh[i]->dataLength << std::endl;
		if (smallestSize == 0 || _lfh[i]->dataLength <= smallestSize) {
			smallestLFH = i;
			smallestSize = _lfh[i]->dataLength;
		}
	}
	if (smallestSize == 0) {
		std::cout << "The largest file is 0bytes....... nothing really interesting to crack !" << std::endl;
		return false;
	}
	_light = new lightLocalFileHeader;
	_light->dataLength = _lfh[smallestLFH]->dataLength;
	_light->checkByte = _lfh[smallestLFH]->checkByte;
	_light->bitFlag = _lfh[smallestLFH]->bitFlag;
	_light->compressionMethod = _lfh[smallestLFH]->compressionMethod;
	_light->crc32 = _lfh[smallestLFH]->crc32;
	//

	std::cout << "SELECTED file: "<< _lfh[smallestLFH]->filename << std::endl;
	_light->checkByte = (_lfh[smallestLFH]->bitFlag & 0x8) ? (_lfh[smallestLFH]->lastModFileTime >> 8) & 0xff : (_lfh[smallestLFH]->crc32 >> 24) & 0xff;
	_file.seekg(_lfh[smallestLFH]->dataStartOffset, std::ios::beg);
	std::cout << "check byte "<<_light->checkByte<<std::endl;;

	_light->data = new char[_light->dataLength];
	_file.read(_light->encryptionHeader, 12);
	_file.read(_light->data, _light->dataLength);
	_tmp_buffer = new unsigned char[_light->dataLength];

	_clean();
	return true;
}

bool		zipCracker::configure(dict& args) {
	_filename = args["--file"].front();
	_file.open(_filename.c_str(), std::ios::in | std::ios::binary);
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
	if (!selectSmallestFile())
		return false;
	return true;
}

bool				zipCracker::crack(char  *passwd){
	size_t 			i;

	initKeys(passwd);
	memcpy(_buffer, _light->encryptionHeader, 12);
	for (i = 0; i < 12; ++i) {
		updateKeys(_buffer[i] ^= decryptByte());
	}
	if (_buffer[11] == _light->checkByte) {		
			memcpy(_tmp_buffer, _light->data, _light->dataLength);
			for ( i = 0; i < _light->dataLength; ++i ) {
				updateKeys(_tmp_buffer[i] ^= decryptByte());
			}
			if (_light->compressionMethod > 0){
				std::cout << "NOT SUPPORTED compression method, cannot check if passwordis correct: "<<passwd<<std::endl;
			} else {
				if (createCrc32(_tmp_buffer, _light->dataLength) == _light->crc32){
					return true;
				}
			}
	}
	return false;
}