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
        cracker->isValid();
        exit(0);
    }
    BruteForce bf(options);
    std::string playground(options->getKey().length(), 'a');
    if (options->getHash())
        playground = "a";
    clock_t start = clock();
    if (!options->getHash())
        returnValue = bf.word(playground, options->getKey().length(), 0, false);
    else
        returnValue = bf.initHash(playground, 0, false);
    if (returnValue == 0)
    {
        double time = (double) (stop - start) / CLOCKS_PER_SEC;
        std::cout << "found it in " << time << " sec" << std::endl;
    }
    delete options;
    delete parser;
    return 0;
}
