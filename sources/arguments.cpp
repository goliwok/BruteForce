#include    "arguments.hpp"

Arguments::Arguments(int ac, char **av)
{
    nbArgs = ac;
    for (int i = 2 ; i != nbArgs ; i++)
        opts.push_back(std::string(av[i]));
    toIterate = { std::make_pair(65, 90), std::make_pair(97, 122), 
                                                std::make_pair(33, 47), std::make_pair(58, 64), 
                                                std::make_pair(91, 96), std::make_pair(123, 126),
                                                std::make_pair(48, 57)};
}

Arguments::~Arguments() 
{

}

bool Arguments::in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

int    Arguments::checkInput(std::string input)
{
    bool contain;

    for (std::string::iterator itstr = input.begin(); itstr != input.end(); itstr++)
    {
        contain = false;
        for (std::vector<std::pair<int , int>>::iterator it = toIterate.begin(); it != toIterate.end(); it++)
            if (*itstr >= it->first && *itstr <= it->second)
                contain = true;
        if (!contain)
            return 1;
    }
    return 0;
}

/*
 by default, digits, letters and symbols are used. with options you can aim for example
 digits and symbols. but you if you don't include the good flags with your input the program will exist
 */
void    Arguments::manageArgs()
{
    if (!in_array("-d", opts))
        toIterate.erase(toIterate.begin()+6, toIterate.begin()+7);
    if (!in_array("-s", opts))
        toIterate.erase(toIterate.begin()+2, toIterate.begin()+6);
    if (!in_array("-l", opts))
        toIterate.erase(toIterate.begin(), toIterate.begin()+2);
}

void    Arguments::parseArguments(std::string key)
{
    if (in_array("-hash", opts))
    {
        hashType = opts[1];
        isHash = true;
        return;
    }
    manageArgs();
    if (checkInput(key) == 1)
    {
        std::cout << "Bad input, you have to set the good option according to your input " << 
        "or I will not be able to find something " << std::endl;
        delete this;
        exit (1);
    }
}

std::string     &Arguments::getHashType()
{
    return hashType;
}

void     Arguments::setHash(bool isHash)
{
    this->isHash = isHash;
}

bool     Arguments::getHash()
{
    return this->isHash;
}

std::vector<std::pair<int, int>> &Arguments::getToIterate()
{
    return toIterate;
}