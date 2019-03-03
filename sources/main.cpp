#include "bruteForce.hpp"
#include "zipCracker.hpp"
#include "arguments.hpp"

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
    Arguments *options = new Arguments(parser);
    std::string key = parser.retrieve<std::string>("Input");
    options->parseArguments(key);
    if (parser.exists("zip")){
        zipCracker *cracker = new zipCracker(key);
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
