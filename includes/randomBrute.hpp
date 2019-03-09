/**
    randomBrute.cpp
    Purpose: stupid random string generator
    @author Louis JURCZYK
    @version 1.0 08/03/19
*/

#ifndef         _RANDOMBRUTE_HPP_
#define         _RANDOMBRUTE_HPP_

#include        "IBrute.hpp"
#include		"benchMark.hpp"

class   randomBrute: public IBrute
{
public:
	bool	configure(dict& options);
    bool    brute(void);


private:
	const std::string&		_recurs(const std::string& str);

	size_t		_maxLength;
	static const std::string	_range;

	bool		_finished;
	std::string				_savedPassword;
};

#endif      /*_RANDOMBRUTE_HPP_*/