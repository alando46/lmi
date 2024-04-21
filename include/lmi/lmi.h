#ifndef LMI_LMI_H
#define LMI_LMI_H
#include <cpr/cpr.h>
#include <jsoncons/json.hpp>
#include <string>
#include <cstdlib> // for getenv
#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>


#include "lmi/client.h"
#include "lmi/api.h"

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#endif


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
    void tryCreate(jsoncons::json& rawJson, std::vector<std::unique_ptr<lmi::LMIFunction>>& validResponses, std::string& errorPrompt) {
        try {
            auto instance = T::create(rawJson);  // create returns std::unique_ptr<T>

            // Properly cast from std::unique_ptr<T> to std::unique_ptr<lmi::LMIFunction>
            std::unique_ptr<lmi::LMIFunction> basePtr = std::unique_ptr<lmi::LMIFunction>(static_cast<lmi::LMIFunction*>(instance.release()));

            validResponses.push_back(std::move(basePtr));
        } catch (const std::exception& e) {
            const char* typeName = typeid(T).name();
            const char* readableName = typeName;  // Assume name is readable by default

            #if defined(__GNUC__) || defined(__clang__)
            int status;
            char* demangled = abi::__cxa_demangle(typeName, nullptr, nullptr, &status);
            if (status == 0) {
                readableName = demangled;  // Use demangled name if demangling was successful
            }
            #elif defined(_MSC_VER)
            readableName = typeName;  // MSVC typically does not require demangling
            #endif

            errorPrompt += "Failed to create instance of " + std::string(readableName) + ": " + e.what() + "\n\n";

            #if defined(__GNUC__) || defined(__clang__)
            std::free(demangled);  // Free the memory allocated by __cxa_demangle
            #endif
        }
    }


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
        // for each class passed to function template, get its schema and transform
        // to openAI Schema
        (toolSchemas.emplace_back(lmi::openAISchemaTransform(Classes::getSchema())), ...);

        std::vector<std::unique_ptr<lmi::LMIFunction>> validResponses;
        int attempt = 1;

        jsoncons::json pbody;
        pbody["model"] = model;
        pbody["messages"] = messages;
        pbody["tools"] = toolSchemas;
        pbody["tool_choice"] = "auto";
        pbody["response_format"] = jsoncons::json::object();
        pbody["response_format"]["type"] = "json_object";


        std::cout << "TEST TEST TEST TEST TEST TEST TEST" << std::endl;
        std::string errorPrompt;

        while (validResponses.size() == 0 && validResponses.size() < maxValidResponses && attempt < maxRetries) {
            // if there were validation errors on the previous attempt, return them to model when it tries again
            if (!errorPrompt.empty()) {
                std:string tmpPrompt = pbody["messages"][0]["content"].as<std::string>();
                pbody["messages"][0]["content"] = (errorPrompt + " Try again. " + tmpPrompt);
                // pbody["messages"][0]["content"] + tmpPrompt;
                std::cout << "validation errors prepended to prompt" << std::endl;
                std::cout << pbody["messages"][0]["content"] << std::endl;
            }

            // make request
            // todo: load all openai-specific things from configuration system
            cpr::Response r = cpr::Post(
                cpr::Url{"https://api.openai.com/v1/chat/completions"},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{pbody.as<std::string>()},
                cpr::Bearer{apiKey}
            );
            // reset error prompt each iteration
            errorPrompt.clear();

            // extract just tool calls from response
            jsoncons::json rj = jsoncons::json::parse(r.text);
            // std::cout << "JSON response: " << rj << std::endl;

            jsoncons::json rawJson = rj["choices"][0]["message"]["tool_calls"];

            // std::cout <<"rawJson is : " << std::endl;

            // std::cout << rawJson << std::endl << std::endl;

            // std::cout <<"tool_calls: " << std::endl;
            // std::cout <<rj["choices"][0]["message"]["tool_calls"][0]<<std::endl;

            auto tryCreateLambda = [&rawJson, &validResponses, &errorPrompt](auto wrapper) {
                for (jsoncons::json func : rawJson.array_range()) {
                    jsoncons::json funcArgs = jsoncons::json::parse(func["function"]["arguments"].as<std::string>());
                    using T = typename decltype(wrapper)::type;
                    tryCreate<T>(funcArgs, validResponses, errorPrompt);

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
