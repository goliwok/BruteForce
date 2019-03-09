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
	_data = NULL;
	_buff = NULL;
	_lightLFH = NULL;
	_encryptionHeader = new char[12];
	_buffer	= new uint8_t[12];

}                                          

zipCracker::~zipCracker() {
	if (_file.is_open())
		_file.close();

	delete[] _encryptionHeader;
	delete[] _buffer;
	if (_data)
		delete[] _data;
	if (_buff)
		delete[] _buff;
	for (auto i: _lfh)
		delete i;
	for (auto i: _cd)
		delete i;
	//delete[] _lightLFH;
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
	_lightLFH = _lfh[smallestLFH];

/*	_lightLFH->crc32 = _lfh[smallestLFH]->crc32;
	_lightLFH->dataLength = _lfh[smallestLFH]->dataLength;
	_lightLFH->bitFlag = _lfh[smallestLFH]->bitFlag;
	_lightLFH->lastModFileTime = _lfh[smallestLFH]->lastModFileTime;
	_lightLFH->compressionMethod = _lfh[smallestLFH]->compressionMethod;
*/
	//std::cout << "SELECTED file: "<< _lightLFH->filename << std::endl;
	_lightLFH->checkByte = (_lightLFH->bitFlag & 0x8) ? (_lightLFH->lastModFileTime >> 8) & 0xff : (_lightLFH->crc32 >> 24) & 0xff;
	_file.seekg(_lfh[smallestLFH]->dataStartOffset, std::ios::beg);
	std::cout << "encryption header start at: "<< _file.tellg() <<" check byte: "<<_lightLFH->checkByte<<std::endl;;
	_file.read(_encryptionHeader, 12);
	_data	= new char[_lightLFH->dataLength];
	_buff	= new unsigned char[_lightLFH->dataLength];
	_file.read(_data, _lightLFH->dataLength);
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
	if (!selectSmallestFile()) {
		return false;
	}
	return true;
}

bool					zipCracker::crack(char  *passwd){
	size_t 				i;
	uint8_t				* buffer         = new uint8_t[12];
	
	//memset(_buffer,0, 12);
	initKeys(passwd);
	memcpy(buffer, _encryptionHeader, 12);
	for (i = 0; i < 12; ++i) { updateKeys(buffer[i] ^= decryptByte()); }
	if (buffer[11] == _lightLFH->checkByte) {
		std::cout <<"HEREEEEEEE"<<std::endl;
		//std::cout << "Motde passe PEUT-ETRE trouvé: "<< passwd<<std::endl;
		memcpy(_buff, _data, _lightLFH->dataLength);
		for ( i = 0; i < _lightLFH->dataLength; ++i ) {
			updateKeys(_buff[i] ^= decryptByte());
		}
		std::cout <<"lll"<<std::endl;
		if ( _lightLFH->compressionMethod == 0) {
			std::cout <<"check crc"<<std::endl;
			if ( createCrc32(_buff, _lightLFH->dataLength) == _lightLFH->crc32 ) {
					std::cout << "HOURA" << passwd<<std::endl;
					return true;
			}
		}
	}
	delete[] buffer;
	return false;
}