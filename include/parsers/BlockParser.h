#pragma once
#include "../utils/JsonWrapper.h" 
#include "../models/Block.h" 
#include <string>
#include <vector>
#include <optional>

class BlockParser {
public:
    static std::optional<Block> parse(const Json& json);  
};
