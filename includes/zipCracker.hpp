#ifndef         _ZIPCRACKER_HPP_
#define         _ZIPCRACKER_HPP_

#include		<iostream>
#include		<fstream>

class           zipCracker
{
    public:
        zipCracker(const std::string& filename);
        ~zipCracker();

        bool				isValid(void);

    private:
    	bool				_checkHeader(void);
    	bool 				_getCentralDirectory(void);

    	const std::string	_filename;
    	std::ifstream		_file;
    	size_t				_start, _end;
};

#endif      /*_ZIPCRACKER_HPP_*/