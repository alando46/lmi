#ifndef LMI_LMI_H
#define LMI_LMI_H
#include <cpr/cpr.h>
#include <jsoncons/json.hpp>
#include <string>
#include <cstdlib> // for getenv
#include <iostream>


#include "lmi/api.h"

namespace lmi {

    inline std::string getTokenFromEnv(const std::string& varName) {
        const char* token = std::getenv(varName.c_str());
        if (token == nullptr) {
            std::cerr << "Error: Environment variable " << varName << " not set.\n";
            return "";
        }
        return std::string(token);
    };


    template<typename T>
    struct type_wrapper {
        using type = T;
    };

    template<typename T>
    void tryCreate(jsoncons::json& rawJson, std::vector<std::unique_ptr<lmi::LMIFunction>>& validResponses) {
        try {
            auto instance = T::create(rawJson);
            validResponses.push_back(std::move(instance));
        } catch (const std::exception& e) {
            std::cout << "Failed to create instance of " << typeid(T).name() << ": " << e.what() << std::endl;
        }
    };

    // template<typename T, typename... Rest>
    template<typename... Classes>
    std::vector<std::unique_ptr<LMIFunction>> makeOAIRequest(
            const std::string& model,
            const std::string& prompt,
            int maxValidResponses=1,
            int maxRetries=3) {

        // get api key from shell
        std::string apiKey = getTokenFromEnv("OAI_API_KEY");

        // build message datastruct with prompt
        jsoncons::json message;
        message["role"] = "user";
        message["content"] = prompt;
        jsoncons::json messages = jsoncons::json::array();
        messages.push_back(message);

        // get schemas from each response model
        jsoncons::json toolSchemas = jsoncons::json::array();
        jsoncons::json function;
        (toolSchemas.emplace_back(Classes::getSchema()), ...);

        // std::string req_messages = messages.as<std::string>();
        // std::string req_toolSchemas = toolSchemas.as<std::string>();

        std::vector<std::unique_ptr<lmi::LMIFunction>> validResponses;
        int attempt = 1;

        std::cout << "model is " << model << std::endl;


        // cpr::Payload payload = {
        //     cpr::Pair{"model", model},
        //     cpr::Pair{"messages", messages.as<std::string>()},
        //     cpr::Pair{"tools", toolSchemas.as<std::string>()},
        //     cpr::Pair{"tool_choice", "auto"}
        // };

        // std::cout << "payload is { ";
        // std::cout << "model: " << model << ", ";
        // std::cout << "messages: " << messages.as<std::string>() << ", ";
        // std::cout << "tools: " << toolSchemas.as<std::string>() << ", ";
        // std::cout << "tool_choice: auto ";
        // std::cout << "}" << std::endl << std::endl;


        jsoncons::json pbody;
        pbody["model"] = model;
        pbody["messages"] = messages;
        pbody["tools"] = toolSchemas;
        pbody["tool_choice"] = "auto";


        std::cout << "body is: " << std::endl << std::endl;
        std::cout << pbody.as<std::string>() << std::endl;

        while (validResponses.size() == 0 && (validResponses.size() < maxValidResponses) && attempt < maxRetries) {
            // make request
            cpr::Response r = cpr::Post(
                cpr::Url{"https://api.openai.com/v1/chat/completions"},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{pbody.as<std::string>()},
                cpr::Bearer{apiKey}
            );

            std::cout << r.status_code << std::endl;
            std::cout << r.url << std::endl;
            std::cout << "content type is: ";
            std::cout << r.header["content-type"] << std::endl;

            // extract just tool calls from response
            jsoncons::json rj = jsoncons::json::parse(r.text);
            std::cout << "JSON response: " << rj << std::endl;

            jsoncons::json rawJson = rj["choices"][0]["message"]["tool_calls"];

            std::cout <<"rawJson is : " << std::endl;

            std::cout << rawJson << std::endl << std::endl;

            auto tryCreateLambda = [&rawJson, &validResponses](auto wrapper) {
                for (jsoncons::json func : rawJson.array_range()) {
                    using T = typename decltype(wrapper)::type;
                    tryCreate<T>(func, validResponses);
                };
            };

            // Expanding pack to try creating each class type
            (tryCreateLambda(type_wrapper<Classes>{}), ...);

            attempt += 1;
            std::cout << "attempt " << attempt << std::endl;
        };

        if (validResponses.size() > 0 && validResponses.size() <= maxValidResponses) {
            return validResponses;
        }

        // else {
        //     throw some exception
        // }
    }
} // namespace lmi

#endif
