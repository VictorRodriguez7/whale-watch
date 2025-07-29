#include "../../include/config/ConfigManager.h"
#include <getopt.h>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

void printUsage() {
    std::cout << "Usage: whale-watch [OPTIONS]\n"
              << "  --threshold <ETH>       Minimum ETH amount for whale (default: 1)\n"
              << "  --rpc-url <URL>         Ethereum RPC endpoint\n"
              << "  --blocks <N>            Number of blocks to scan (default: 10)\n"
              << "  --continuous            Enable continuous scanning mode\n"
              << "  --help                  Show this help message\n";
}

Config parseConfig(int argc, char* argv[]) {
    Config config{1.0, "", 10, false};

    static struct option long_options[] = {
        {"threshold", required_argument, nullptr, 't'},
        {"rpc-url", required_argument, nullptr, 'r'},
        {"blocks", required_argument, nullptr, 'b'},
        {"continuous", no_argument, nullptr, 'c'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "t:r:b:ch", long_options, nullptr)) != -1) {
        switch (opt) {
            case 't': config.thresholdEth = std::stod(optarg); break;
            case 'r': config.rpcUrl = optarg; break;
            case 'b': config.numBlocks = std::stoi(optarg); break;
            case 'c': config.continuous = true; break;
            case 'h': printUsage(); exit(0);
            default: printUsage(); exit(1);
        }
    }
    
    if (config.rpcUrl.empty()) {
        const char* rpcEnv = std::getenv("RPC_URL");
        if (rpcEnv) config.rpcUrl = rpcEnv;
    }

    if (config.rpcUrl.empty()) {
        throw std::runtime_error("RPC URL is required (use --rpc-url or .env)");
    }

    return config;
}
