#include "bruteForce.hpp"
#include "arguments.hpp"
#include <typeinfo>
#include <openssl/sha.h>
#include <openssl/md5.h>

BruteForce::BruteForce()
{
    options = _options;
    key = _options->getKey();
    playground = std::string(options->getKey().length(), 'a');
    if (options->getHash())
        playground = "a";
    std::string playground(options->getKey().length(), 'a');
    std::cout << "max depth: " << key.length() << std::endl;
}

BruteForce::~BruteForce()
{

}

std::string     &BruteForce::generateMd5(std::string & input, std::string &hashGen)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    unsigned char text[input.length() + 1];

    for (int i = 0 ; i != input.length() ; i++)
        text[i] = input[i];
    MD5(text, sizeof(text) - 1, hash);
 
    char md5string[MD5_DIGEST_LENGTH * 2 +1];
    for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(&md5string[i * 2], "%02x", (unsigned int) hash[i]);
    hashGen = md5string;
    return hashGen;
} 

std::string     &BruteForce::generateSha1(std::string & input, std::string &hashGen)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    unsigned char text[input.length() + 1];

    for (int i = 0 ; i != input.length() ; i++)
        text[i] = input[i];
    SHA1(text, sizeof(text) - 1, hash);
 
    char sha1string[SHA_DIGEST_LENGTH * 2 +1];
    for(int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        sprintf(&sha1string[i * 2], "%02x", (unsigned int) hash[i]);
    hashGen = sha1string;
    return hashGen;
}    

int             BruteForce::initHash(int currentDepth, bool op)
{
    std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
    if (options->getHashType() == "sha1")
        hashfunc = std::bind(&BruteForce::generateSha1, this, std::placeholders::_1, std::placeholders::_2);
    else
        hashfunc = std::bind(&BruteForce::generateMd5, this, std::placeholders::_1, std::placeholders::_2);      
    return  hash(currentDepth, op);
}

int             BruteForce::hash(int currentDepth, bool op)
{
    for (std::vector<std::pair<int, int>>::iterator it = options->getToIterate().begin(); it != options->getToIterate().end(); ++it)
    {
        for (char charPos = it->first ; charPos != it->second+1 ; charPos++)
        {
            playground[currentDepth] = charPos;
            std::string hashGen;
            if (hashfunc(playground, hashGen).compare(key) == 0) 
            {
                std::cout << "found: " << playground << std::endl;
                stop = clock();
                return (0);
            }
            if (currentDepth != 0)
                if (hash(currentDepth - 1, true) == 0)
                    return (0);
        }
    }
    if (op == true)
        return -1;
    playground = std::string(playground.length()+1, 'a');
    if (hash(currentDepth + 1, false) == 0)
        return 0;
    return -1;
}

int     BruteForce::word(const int maxDepth, int currentDepth, bool op)
{
    for (std::vector<std::pair<int, int>>::iterator it = options->getToIterate().begin(); it != options->getToIterate().end(); ++it)
    {
        for (char charPos = it->first ; charPos != it->second+1 ; charPos++)
        {
            playground[currentDepth] = charPos;
            if (playground.compare(key) == 0)
            {
                std::cout << "found: " << playground << std::endl;
                stop = clock();
                return (0);
            }
            if (currentDepth != 0)
                if (word(maxDepth, currentDepth - 1, true) == 0)
                    return (0);
        }
    }
    if (op == true)
        return -1;    
    else if (currentDepth < maxDepth - 1)
    {   
        playground = std::string(maxDepth, 'a');
        if (word(maxDepth, currentDepth + 1, false) == 0)
            return 0;
    }
    return -1;    
}