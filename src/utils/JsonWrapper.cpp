#include "../../include/utils/JsonWrapper.h"
#include "../../include/utils/Logger.h"

Json JsonWrapper::parse(const std::string& jsonStr) {
    try {
        return Json::parse(jsonStr);
    } catch (const std::exception& e) {
        Logger::error("Failed to parse JSON: " + std::string(e.what()));
        return Json{};  // return empty object so code doesn't crash
    }
}
