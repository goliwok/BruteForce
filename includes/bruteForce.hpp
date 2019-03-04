#ifndef     _BRUTEFORCE_HPP_
#define     _BRUTEFORCE_HPP_

#include "arguments.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

static clock_t stop;

class   BruteForce
{
    public:
        BruteForce(Arguments *_options);
        ~BruteForce();

        int     word(const int, int, bool);
        int     hash(int, bool);
        int     initHash(int, bool);

    private:
        std::string     key;
        std::string     playground;
        Arguments       *options;
        std::string     &generateSha1(std::string &, std::string &);
        std::string     &generateMd5(std::string &, std::string &);
        std::function<std::string &(std::string &, std::string &)> hashfunc;
};

#endif      /*_BRUTEFORCE_HPP_*/