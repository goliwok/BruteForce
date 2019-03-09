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

    void        printUInt128(__uint128_t value){
       uint16_t *val = (uint16_t*) &value;
       std::cout <<val[0]<< val[1] << val[2]<< val[3]<< val[4]<< val[5]<<val[6]<< val[7]<<std::endl;
    }
    void        stop(unsigned long long nbTries) {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<double> ts = std::chrono::duration_cast<std::chrono::duration<double>>(now - _clock);
        std::cout << "IT TOOK:"<< ts.count()<< " seconds."<<std::endl;
        std::cout << std::dec<<nbTries<< " password tested."<<std::endl;
        //std::cout << "l"<<nbTries << " password tried."<< std::endl;
    }
private:
    std::chrono::steady_clock::time_point   _clock;
    //__uint128_t     _nb;
};

#endif      /*_BENCHMARK_HPP_*/ 