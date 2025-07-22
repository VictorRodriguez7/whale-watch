#pragma once
#include <string>
#include <optional>

struct Transaction {
    std::string hash;
    std::string from;
    std::optional<std::string> to;
    std::string value;
};
