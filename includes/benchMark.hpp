/**
    @author Louis JURCZYK
    @version 1.0 10/03/19
*/

#ifndef         _BENCHMARK_HPP_
#define         _BENCHMARK_HPP_

#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
class benchMark
{
public:
    benchMark(){}

    ~benchMark(){}

    void        start() {
        std::cout<< "Starting Timer!!" <<std::endl;
        _clock = std::chrono::steady_clock::now();
    }

    void        stop(__uint128_t nbTries) {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<double> ts = std::chrono::duration_cast<std::chrono::duration<double>>(now - _clock);
        std::cout << "IT TOOK:"<< ts.count()<< std::endl;
    }
private:
    std::chrono::steady_clock::time_point   _clock;
    //__uint128_t     _nb;
};

#endif      /*_BENCHMARK_HPP_*/ 