#ifndef         _ARGUMENTS_HPP_
#define         _ARGUMENTS_HPP_

#include        <iostream>
#include        <vector>
#include        <algorithm>
#include        <functional>
#include        "Lib/argparse.hpp"

class           Arguments
{
    public:
        Arguments(ArgumentParser &parser);
        ~Arguments();

        void        parseArguments(std::string key);
        std::string &getHashType();
        std::vector<std::pair<int, int>> &getToIterate();
        void        setHash(bool);
        bool        getHash();

    private:
        ArgumentParser parser;
        std::vector<std::pair<int, int>>    toIterate;
        std::string                         hashType;
        bool                                isHash;
        std::function<int()>                hashFunction;

        void        manageArgs();
        void        manageArgsHash();
        int         checkInput(std::string input);
        bool        in_array(const std::string &value, const std::vector<std::string> &array);    
};

#endif      /*_ARGUMENTS_HPP_*/