/**
    randomBrute.cpp
    Purpose: stupid random string generator

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"randomBrute.hpp"

randomBrute::randomBrute() {}

randomBrute::~randomBrute() {}

bool		randomBrute::configure(dict& options) {
	std::cout <<"Configuring random brute force" << std::endl;
}

void		randomBrute::brute(){
	_cracker->crack();
}