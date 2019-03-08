/**
    randomBrute.cpp
    Purpose: stupid random string generator

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"randomBrute.hpp"

bool		randomBrute::configure(dict& options) {
	return true;
}

void		randomBrute::brute(){
	_cracker->crack("toto");
}