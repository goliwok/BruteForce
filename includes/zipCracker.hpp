/**
    zipCracker.hpp
    Purpose: brute force encrypted zip files

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#ifndef         _ZIPCRACKER_HPP_
#define         _ZIPCRACKER_HPP_

#include		<fstream>
#include        <stdio.h>
#include        <string.h>
#include		"zipReader.hpp"
#include        "ICracker.hpp"

class           zipCracker: public ICracker {
    public:
        zipCracker();
        ~zipCracker();

        bool                crackOLD(void);
        bool                crack();
        bool                configure(dict& options);

    private:
        bool                isValid(const dict args);
    	bool				_checkHeader(void);
    	bool 				_getEndOfCentralDirectoryOffset(void);
		void 				_initStructures(void);
        void                _aggressiveFindLFH(void);

    	std::string        _filename;
    	std::ifstream		_file;
    	size_t				_eocd_offset;

    	endOfCentralDirectory 				_eocd;
    	std::vector<centralDirectory *>		_cd;
    	std::vector<localFileHeader *>		_lfh;
};

#endif      /*_ZIPCRACKER_HPP_*/