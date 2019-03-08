/**
    @author Louis JURCZYK
    @version 1.0 08/03/19
*/

#ifndef         _IBRUTE_HPP_
#define         _IBRUTE_HPP_

#include        <iostream>
#include        <map>
#include        <memory>
#include        <vector>
#include        "ICracker.hpp"
typedef std::map<std::string, std::vector<std::string>> dict;


class   IBrute
{
public:
    IBrute() {}
    ~IBrute() {}

    virtual bool    	brute(void) = 0;
    virtual bool        configure(dict& options) = 0;

    void			setCracker(std::shared_ptr<ICracker> c) {
    	_cracker = c;
    }

protected:
	std::shared_ptr<ICracker>  _cracker;
};
#endif      /*_IBRUTE_HPP_*/