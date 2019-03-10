/**
    randomBrute.cpp
    Purpose: stupid random string generator
    @author Louis JURCZYK
    @version 1.0 08/03/19
*/

#ifndef         _READSTDIN_HPP_
#define         _READSTDIN_HPP_

#include        "IBrute.hpp"
#include		"benchMark.hpp"
#include		<sstream>
#include		<string>

class   readStdin: public IBrute
{
public:
	bool	configure(dict& options) {}
    bool    brute(void){
        std::string password;

        _start();
        while ((std::cin >> password)) {
            _update();
            if (_cracker->crack(const_cast<char*>(password.c_str()))) {
                _finish();
                std::cout << "found password:" << password;
                return true;
            }
        }
        _finish();
        return false;
    }
};

#endif      /*_READSTDIN_HPP_*/