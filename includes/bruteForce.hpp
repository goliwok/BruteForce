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

        int     word(std::string&, const int, int, bool);
        int     hash(std::string &, int, bool);
        int     initHash(std::string &, int, bool);

    private:
        std::string     key;
        Arguments       *options;
        std::string     &generateSha1(std::string &, std::string &);
        std::string     &generateMd5(std::string &, std::string &);
        std::function<std::string &(std::string &, std::string &)> hashfunc;
};

#endif      /*_BRUTEFORCE_HPP_*/