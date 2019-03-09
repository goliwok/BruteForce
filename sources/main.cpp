#include "argParse.hpp"
#include "zipCracker.hpp"
#include "randomBrute.hpp"
  #include <cmath>


typedef void (*func)(std::string);

int     main (int ac, const char **av)
{
    dict            options;
    argumentParser  *args = new argumentParser(ac, av);
    std::map<std::string, std::shared_ptr<ICracker>> Crackers;
    std::map<std::string, std::shared_ptr<IBrute>> Brutus;

    args->addArgument("-h","--help", 0, "Print this message");
    args->addArgument("-m","--mode", 1, "Choose which bruteforce mode", false);
    args->addArgument("-c","--crack", 1, "Choose what to crakc! zip/md5/...", false);
    args->addArgument("-f","--file", 1, "Requires a zip file as paremeter", true, "zip");
    args->parse();

    options = args->getOptions();
    std::string mode(options.find("--mode")->second.front());
    std::string crack(options.find("--crack")->second.front());

    Crackers["zip"] = std::make_shared<zipCracker>();
    Brutus["random"] = std::make_shared<randomBrute>();

    if (Crackers.find(crack) == Crackers.end())
        args->printHelp("This cracking method (" + crack + ") is not implemented yet:\"(");
    if (Brutus.find(mode) == Brutus.end())
        args->printHelp("This bruteforce method (" + mode + ") is not implemented yet:\"(");
    
    if (Crackers[crack]->configure(options) && Brutus[mode]->configure(options)){
        Brutus[mode]->setCracker(Crackers[crack]);
        delete args;
        //Let's gooooo
        Brutus[mode]->brute();
    } else
        delete args;
    return 0;
}
