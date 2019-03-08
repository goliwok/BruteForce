/**
    @author Louis JURCZYK
    @version 1.0 06/03/19
*/

#ifndef         _ICRACKER_HPP_
#define         _ICRACKER_HPP_

#include		<iostream>
#include		<map>
#include        <vector>

typedef std::map<std::string, std::vector<std::string>> dict;
class   ICracker
{
public:
	ICracker() {}
    ~ICracker(){}


    virtual bool		configure(dict& options) = 0;
    virtual bool 		crack() = 0;
};
#endif      /*_ICRACKER_HPP_*/