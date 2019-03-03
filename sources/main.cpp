#include "bruteForce.hpp"
#include "zipCracker.hpp"
#include "arguments.hpp"
#include "Lib/argparse.hpp"

void    printHelp()
{
    std::cout << 
        "USAGE: ./Bf <key> [-l] [-s] [-d]\n \n -l:\tletters\n -s:\tsymbols\n -d:\tdigits \n\n"
        << "OR ./Bf <hash> -hash <hashType>: \n [sha1] \n [md5]"
        << std::endl;
    exit (1);
}

int     main (int ac, const char **av)
{
    ArgumentParser parser;

    parser.addArgument("-l", "--letters");
    parser.addArgument("-s", "--symbols");
    parser.addArgument("-d", "--digits");
    parser.addArgument("-h", "--hash", 1);
    parser.addArgument("-z", "--zip");
    parser.addFinalArgument("Input");

    parser.parse(ac, av);

    int returnValue = 0;
    std::string key = parser.retrieve<std::string>("Input");
    Arguments *options = new Arguments(parser);
    options->parseArguments(key);
    if (parser.exists("zip")){
        zipCracker *cracker = new zipCracker(av[2]);
        cracker->isValid();
        exit(0);
    }
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
