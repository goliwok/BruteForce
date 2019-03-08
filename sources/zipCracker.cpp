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
}                                          

zipCracker::~zipCracker() {
	for (auto i: _cd)
		delete i;
	for (auto i: _lfh)
		delete i;
	_file.close();
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
	if (_lfh.size() == 0){
		std::cout << "no local file header found" << std::endl;
		return false;
	}
	return true;
}

bool					zipCracker::crack(void){
	std::cout <<"lolje crack"<<std::endl;
}
bool					zipCracker::crackOLD(void) {
	uint32_t			smallestSize = 0;
	uint32_t			smallestLFH;

	for (size_t i = 0; i < _lfh.size(); i++) {
		if (smallestSize == 0 || _lfh[i]->dataLength <= smallestSize){
			smallestLFH = i;
			smallestSize = _lfh[i]->dataLength;
		}
	}
	if (smallestSize == 0) {
		std::cout << "The largest file is 0bytes....... nothing really interesting to crack !" << std::endl;
		return false;
	}
	char 				*encryptionHeader 	= new char[12];
	uint8_t 			*buffer				= new uint8_t[12];

	_file.seekg(_lfh[smallestLFH]->dataStartOffset, std::ios::beg);
	_file.read(encryptionHeader, 12);
	_file.close();

	uint16_t checkByte;
	if (_lfh[smallestLFH]->bitFlag & 0x8) {
		checkByte = (_lfh[smallestLFH]->lastModFileTime >> 8) & 0xff;
		std::cout << "FLAG ISSET"<<std::endl;
	}else{
		checkByte = (_lfh[smallestLFH]->crc32 >> 24) & 0xff;
		std::cout << "FLAG not set"<<std::endl;
	}
	std::cout <<"CHECKBYTE:"<< checkByte <<std::endl;

	initKeys("toto");
	size_t i;
	memcpy(buffer, encryptionHeader, 12);
	for ( i = 0; i < 12; ++i ) {
		std::cout << "||" << +buffer[i];
	}
	std::cout << std::endl;
	for ( i = 0; i < 12; ++i ) {
		updateKeys(buffer[i] ^= decryptByte());
	}

	uint8_t check_crc = _lfh[smallestLFH]->crc32 >> 24;
	uint8_t check_deflated	=  _lfh[smallestLFH]->lastModFileTime >> 8;
	for ( i = 0; i < 12; ++i ) {
		std::cout << "||" << +buffer[i];
	}
	std::cout << "\ncheck_crc:" << +check_crc << "\ncheck deflated:"<< +check_deflated<<std::endl;

	delete[] encryptionHeader;
	delete[] buffer;
	return true;
}