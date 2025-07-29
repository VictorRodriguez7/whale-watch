#include "../../include/utils/RetryUtils.h"
#include "../../include/utils/Logger.h"


void retryWithBackoff(const std::function<void()>& func,
                      int maxRetries,
                      const std::string& operationName) {
    int retries = 0;

    while (true) {
        try {
            func();
            return; 
        } catch (const std::exception& e) {
            retries++;
            Logger::error("Error during " + operationName + ": " + e.what());

            if (retries > maxRetries) {
                Logger::error("Max retries reached for " + operationName + ". Giving up.");
                throw; 
            }

            int waitTime = std::pow(2, retries);
            Logger::info("Retrying " + operationName + " in " +
                         std::to_string(waitTime) + " seconds (attempt " +
                         std::to_string(retries) + "/" + std::to_string(maxRetries) + ")...");
            std::this_thread::sleep_for(std::chrono::seconds(waitTime));
        }
    }
}
