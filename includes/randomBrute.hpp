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
    randomBrute();
    ~randomBrute();
	
	bool	configure(dict& options);
    void    brute(void);
};
#endif      /*_RANDOMBRUTE_HPP_*/