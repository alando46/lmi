#ifndef LMI_LMI_H
#define LMI_LMI_H
#include <cpr/cpr.h>
#include <jsoncons/json.hpp>

#include "lmi/api.h"

namespace lmi {

    template<typename T, typename... Rest>
    jsoncons::json getSchemaFromModels() {


        return T::getSchema();
    }

    // template<typename T, typename... Rest>
    template<typename... Classes>
    std::unique_ptr<LMIFunction> makeOAIRequest(
            const std::string& apiKey,
            const std::string& model,
            const std::string& prompt,
            int maxValidResponses=1,
            int maxRetries=3) {

        // build message datastruct with prompt
        jsoncons::json message;
        message["role"] = "user";
        message["content"] = prompt;
        jsoncons::json messages = jsoncons::json::array();
        messages.push_back(message);

        // get schemas from each response model
        jsoncons::json toolSchemas = jsoncons::json::array();
        (toolSchemas.emplace_back(Classes::getSchema(), ...);

        std::vector<std::unique_ptr<lmi::LMIFunction>> validResponses;
        int attempt = 1;

        while (validResponses.size() == 0 || (validResponses.size() > maxValidResponses) && attempt < maxRetries) {
            // make request
            cpr::Response r = cpr::Post(
                cpr::Url{"https://api.openai.com/v1/chat/completions"},
                cpr::Payload{
                    {"model", model},
                    {"messages", message.as<std::string>},
                    {"tools", toolSchemas.as<std::string>}
                }, cpr::Bearer{apiKey}
            )

            // extract just tool calls from response
            jsoncons::json j = jsoncons::json::parse(r.text);
            std::cout << "JSON response: " << j << std::endl;

            // Helper lambda to attempt creation of each type and catch failures
            auto tryCreate = [&rawJson, &validResponses](auto dummy) {
                using T = decltype(dummy);
                try {
                    auto validResponses = T::create(rawJson);
                    if (instance) {
                        validResponses.push_back(std::move(validResponses));
                    }
                } catch (const std::exception& e) {
                    std::cout << "Failed to create instance of " << typeid(T).name() << ": " << e.what() << std::endl;
                }
            };

            // Expanding pack to try creating each class type
            (tryCreate(Classes{}), ...);

            attempt += 1;
            }


        // validate response with each of the classes
        // if at most 1 is valid, return

    }
} // namespace lmi

#endif
