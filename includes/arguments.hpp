#ifndef         _ARGUMENTS_HPP_
#define         _ARGUMENTS_HPP_

#include        <iostream>
#include        <vector>
#include        <algorithm>
#include        <functional>

class           Arguments
{
    public:
        Arguments(int, char **);
        ~Arguments();

        void        parseArguments(std::string key);
        std::string &getHashType();
        std::vector<std::pair<int, int>> &getToIterate();
        void        setHash(bool);
        bool        getHash();
        bool        isZip() {return !zipFile.empty();};

    private:
        std::vector<std::string>            opts;
        int                                 nbArgs;
        std::vector<std::pair<int, int>>    toIterate;
        std::string                         hashType;
        bool                                isHash;
        std::string                         zipFile;

        std::function<int()> hashFunction;

        void        manageArgs();
        void        manageArgsHash();
        int         checkInput(std::string input);
        bool        in_array(const std::string &value, const std::vector<std::string> &array);    
};

#endif      /*_ARGUMENTS_HPP_*/