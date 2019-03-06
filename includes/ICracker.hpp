/**
    @author Louis JURCZYK
    @version 1.0 06/03/19
*/

#ifndef         _ICRACKER_HPP_
#define         _ICRACKER_HPP_

class   ICracker
{
public:
    ICracker();
    ~ICracker();

    virtual bool        init(const char *params...) = 0;
    virtual bool        crack(const char *password) = 0;
};
#endif      /*_ICRACKER_HPP_*/