#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

using Json = nlohmann::json;

class JsonWrapper {
public:
    static Json parse(const std::string& jsonStr);
};
