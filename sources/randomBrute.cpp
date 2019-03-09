/**
    randomBrute.cpp
    Purpose: stupid random string generator

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"randomBrute.hpp"

//const std::string randomBrute::_range = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const std::string randomBrute::_range = "to";
bool		randomBrute::configure(dict& options) {
	_maxLength = 4;
	return true;
}

bool		randomBrute::_recurs(const std::string& str) {
	if (_cracker->crack(const_cast<char*>(str.c_str())) || str.length() == _maxLength){
		return true;
	} else {
		for(auto c : _range) {
			std::string next = str + c;
			std::cout << next <<  std::endl;
			_recurs(next);
      	}
	}
}

bool		randomBrute::brute(){
	return _recurs("");
}