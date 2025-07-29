#pragma once
#include <string>

struct Config {
    double thresholdEth;
    std::string rpcUrl;
    int numBlocks;
    bool continuous;
};

Config parseConfig(int argc, char* argv[]);
