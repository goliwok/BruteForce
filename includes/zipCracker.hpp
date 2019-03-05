/**
    zipCracker.hpp
    Purpose: brute force encrypted zip files

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#ifndef         _ZIPCRACKER_HPP_
#define         _ZIPCRACKER_HPP_

#include		<iostream>
#include		<fstream>
#include		<vector>
#include		"zipReader.hpp"

class           zipCracker
{
    public:
        zipCracker(const std::string& filename);
        ~zipCracker();

        bool				isValid(void);

    private:
    	bool				_checkHeader(void);
    	bool 				_getEndOfCentralDirectoryOffset(void);
		void 				_initStructures(void);
        void                _aggressiveFindLFH(void);

    	const std::string	_filename;
    	std::ifstream		_file;
    	size_t				_eocd_offset;

    	endOfCentralDirectory 				_eocd;
    	std::vector<centralDirectory *>		_cd;
    	std::vector<localFileHeader *>		_lfh;
};

#endif      /*_ZIPCRACKER_HPP_*/