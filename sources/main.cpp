#include "bruteForce.hpp"
#include "zipCracker.hpp"
#include "arguments.hpp"
#include "Lib/argparse.hpp"

ArgumentParser *parse(int ac, const char **av)
{
    ArgumentParser *parser = new ArgumentParser;

    parser->addArgument("-l", "--letters");
    parser->addArgument("-s", "--symbols");
    parser->addArgument("-d", "--digits");
    parser->addArgument("-h", "--hash", 1);
    parser->addArgument("-z", "--zip");
    parser->addFinalArgument("Input");

    if (parser->parse(ac, av) == -1)
    {
        delete parser;
        exit(-1);
    }
    return parser;
}

int     main (int ac, const char **av)
{
    int returnValue = 0;
    ArgumentParser *parser = parse(ac, av);
    Arguments *options = new Arguments(parser);
    options->parseArguments();
    if (parser->exists("zip")) {
        zipCracker *cracker = new zipCracker(options->getKey());
        if (!cracker->isValid()) 
            exit(1);
        if (!cracker->crack());
        exit(0);
    }
    BruteForce bf(options);
    clock_t start = clock();
    if (!options->getHash())
        returnValue = bf.word(options->getKey().length(), 0, false);
    else
        returnValue = bf.initHash(0, false);
    if (returnValue == 0)
        std::cout << "found it in " << (double) (stop - start) / CLOCKS_PER_SEC << " sec" << std::endl;
    delete options;
    delete parser;
    return 0;
}
