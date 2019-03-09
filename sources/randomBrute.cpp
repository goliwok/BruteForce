/**
    randomBrute.cpp
    Purpose: stupid random string generator

    @author Louis JURCZYK
    @version 1.0 03/03/19
*/

#include	"randomBrute.hpp"

const std::string randomBrute::_range = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
//const std::string randomBrute::_range = "azertyuipqsdfghjklm1234567890+-@";
bool		randomBrute::configure(dict& options) {
	_maxLength = 6;
	return true;
}

const std::string&	randomBrute::_recurs(const std::string& str) {
	if (_cracker->crack(const_cast<char*>(str.c_str()))){
		_finish();
		_finished = true;
		_savedPassword = str;
		return str;
	}
	if (str.length() == _maxLength || _finished){
		return "";
	} else {
			_update();
		for(auto c : _range) {
			std::string next = str + c;
			_recurs(next);
      	}
	}
}

bool		randomBrute::brute(){
	std::string	password;

	_start();
	password = _recurs("");

	if (_finished) {
		std::cout <<"Found passwd!!->" << _savedPassword <<std::endl;
		return true; 
	}
	_finish();
	std::cout <<"Password not found"<<std::endl;
	return false;
}