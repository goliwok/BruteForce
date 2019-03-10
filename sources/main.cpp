#include "argParse.hpp"
#include "zipCracker.hpp"
#include "randomBrute.hpp"
#include "readStdin.hpp"
#include <csignal>

typedef void (*func)(std::string);

// ouille
std::map<std::string, std::shared_ptr<IBrute>> Brutus;
std::shared_ptr<IBrute>     bruteur;

void    signal_handler(int signal){
    bruteur->stop();
    exit(0);
}

int     main (int ac, const char **av)
{
    std::signal(SIGINT, signal_handler);
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
    Brutus["stdin"] = std::make_shared<readStdin>();


    if (Crackers.find(crack) == Crackers.end())
        args->printHelp("This cracking method (" + crack + ") is not implemented yet:\"(");
    if (Brutus.find(mode) == Brutus.end())
        args->printHelp("This bruteforce method (" + mode + ") is not implemented yet:\"(");

    bruteur = Brutus[mode];    
    if (Crackers[crack]->configure(options) && bruteur->configure(options)){
        bruteur->setCracker(Crackers[crack]);
        delete args;
        //Let's gooooo
        bruteur->brute();
    } else
        delete args;
    return 0;
}
