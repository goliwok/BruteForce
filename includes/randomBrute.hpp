/**
    randomBrute.cpp
    Purpose: stupid random string generator
    @author Louis JURCZYK
    @version 1.0 08/03/19
*/

#ifndef         _RANDOMBRUTE_HPP_
#define         _RANDOMBRUTE_HPP_

#include        "IBrute.hpp"

class   randomBrute: public IBrute
{
public:
	bool	configure(dict& options);
    bool    brute(void);


private:
	bool		_recurs(const std::string& str);
	size_t		_maxLength;
	std::string	_buffer;

public:
	static const std::string _range;
};

#endif      /*_RANDOMBRUTE_HPP_*/