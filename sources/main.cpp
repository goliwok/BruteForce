#include "bruteForce.hpp"
#include "arguments.hpp"

void    printHelp()
{
    std::cout << 
        "USAGE: ./Bf <key> [-l] [-s] [-d]\n \n -l:\tletters\n -s:\tsymbols\n -d:\tdigits \n\n"
        << "OR ./Bf <hash> -hash <hashType>: \n [sha1] \n [md5]"
        << std::endl;
    exit (1);
}

int     main (int ac, char **av)
{
    int returnValue = 0;
    if (ac < 2)
        printHelp();
    std::string key = std::string(av[1]);        
    Arguments *options = new Arguments(ac, av);
    options->setHash(false);
    if (ac > 2)
        options->parseArguments(key);    
    BruteForce bf(options, key);
    std::string playground(key.length(), 'a');
    if (options->getHash())
        playground = "a";
    clock_t start = clock();
    if (!options->getHash())
        returnValue = bf.word(playground, key.length(), 0, false);
    else
        returnValue = bf.initHash(playground, 0, false);
    if (returnValue == 0)
    {
        double time = (double) (stop - start) / CLOCKS_PER_SEC;
        std::cout << "found it in " << time << " sec" << std::endl;
    }
    delete options;
    return 0;
}
