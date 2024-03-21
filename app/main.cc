// #define FMT_HEADER_ONLY // may need this line

#include <filesystem>
#include <fstream>
#include <iostream>

// #include <cxxopts.hpp>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <boost/hana.hpp>
#include <cpr/cpr.h>


#include "config.hpp"
#include "validator.h"
#include "base.h"
#include "alt.h"
using json = nlohmann::json;
namespace fs = std::filesystem;
namespace hana = boost::hana;

int main(int argc, char **argv)
{

    alt();

    std::cout << "JSON: " << NLOHMANN_JSON_VERSION_MAJOR << "."
              << NLOHMANN_JSON_VERSION_MINOR << "."
              << NLOHMANN_JSON_VERSION_PATCH << '\n';

    std::cout << "FMT: " << FMT_VERSION << '\n';
    // std::cout << "CXXOPTS: " << CXXOPTS__VERSION_MAJOR << "."
    //           << CXXOPTS__VERSION_MINOR << "." << CXXOPTS__VERSION_PATCH
    //           << '\n';
    std::cout << "SPDLOG: " << SPDLOG_VER_MAJOR << "." << SPDLOG_VER_MINOR
              << "." << SPDLOG_VER_PATCH << '\n';

    // create an empty structure (null)
    json bobJson;
    bobJson["name"] = "bob";
    bobJson["age"] = 18;
    bobJson["height"] = 2.5;

    // cpr::Response r = cpr::Get(cpr::Url{"http://www.httpbin.org/get"});

    // std::cout << r.url << std::endl; // http://www.httpbin.org/get
    // std::cout << r.status_code << std::endl; // 200
    // std::cout << r.header["content-type"] << std::endl; // application/json
    // std::cout << r.text << std::endl;


    // Person bob{bobJson};

    // std::cout << to_json(bob) << std::endl;

    // Person bill{"bill", 15, 2.5};
    float height = 2.5;
    auto bill = Person::create("bill", 15, 2.0);
    std::cout << to_json(bill) << std::endl;


    // Your OpenAI API key
    std::string apiKey = "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";

    // Headers
    cpr::Header headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + apiKey}
    };

    // Constructing the JSON payload using nlohmann::json
    nlohmann::json jsonPayload = {
        {"model", "gpt-3.5-turbo"},
        {"messages", {
            {
                {"role", "user"},
                {"content", "What is the weather like in Boston?"}
            }
        }},
        {"tools", {
            {
                {"type", "function"},
                {"function", {
                    {"name", "get_current_weather"},
                    {"description", "Get the current weather in a given location"},
                    {"parameters", {
                        {"type", "object"},
                        {"properties", {
                            {"location", {
                                {"type", "string"},
                                {"description", "The city and state, e.g. San Francisco, CA"}
                            }},
                            {"unit", {
                                {"type", "string"},
                                {"enum", {"celsius", "fahrenheit"}}
                            }}
                        }},
                        {"required", {"location"}}
                    }}
                }}
            }
        }},
        {"tool_choice", "auto"}
    };
    // Make the POST request
    cpr::Response r = cpr::Post(
        cpr::Url{"https://api.openai.com/v1/chat/completions"},
        headers,
        cpr::Body{jsonPayload.dump()} // Convert JSON object to string format
    );
    std::cout << r.text << std::endl;
    // Check the response status and print the response text
    if (r.status_code == 200) {
        // Parse the response text into a JSON object
        nlohmann::json responseJson = nlohmann::json::parse(r.text);
        std::cout<< "hihihihi" << std::endl;
        std::cout << responseJson << std::endl;

        // Extract function calls from the response
        if (!responseJson["choices"].empty() && !responseJson["choices"][0]["message"]["tool_calls"].empty()) {
            auto toolCalls = responseJson["choices"][0]["message"]["tool_calls"];

            for (auto& call : toolCalls) {
                // Each `call` is now a nlohmann::json object representing a function call
                std::cout << "Function Call: " << call.dump(4) << std::endl;
            }
        } else {
            std::cout << "No function calls found in the response." << std::endl;
        }
    } else {
        std::cout << "Failed to get response. Status code: " << r.status_code << std::endl;
    }

    return 0;
}
//     // std::cout << "FMT: " << FMT_VERSION << '\n';
//     // std::cout << "CXXOPTS: " << CXXOPTS__VERSION_MAJOR << "."
//     //           << CXXOPTS__VERSION_MINOR << "." << CXXOPTS__VERSION_PATCH
//     //           << '\n';
//     // std::cout << "SPDLOG: " << SPDLOG_VER_MAJOR << "." << SPDLOG_VER_MINOR
//     //           << "." << SPDLOG_VER_PATCH << '\n';
//     std::cout << "\n\nUsage Example:\n";

//     // Compiler Warning and clang tidy error
//     // std::int32_t i = 0;

//     // Adress Sanitizer should see this
//     // int *x = new int[42];
//     // x[100] = 5; // Boom!

//     const auto welcome_message =
//         fmt::format("Welcome to {} v{}\n", project_name, project_version);
//     spdlog::info(welcome_message);

//     cxxopts::Options options(project_name.data(), welcome_message);

//     options.add_options("arguments")("h,help", "Print usage")(
//         "f,filename",
//         "File name",
//         cxxopts::value<std::string>())(
//         "v,verbose",
//         "Verbose output",
//         cxxopts::value<bool>()->default_value("false"));

//     auto result = options.parse(argc, argv);

//     if (argc == 1 || result.count("help"))
//     {
//         std::cout << options.help() << '\n';
//         return 0;
//     }

//     auto filename = std::string{};
//     auto verbose = false;

//     if (result.count("filename"))
//     {
//         filename = result["filename"].as<std::string>();
//     }
//     else
//     {
//         return 1;
//     }

//     verbose = result["verbose"].as<bool>();

//     if (verbose)
//     {
//         fmt::print("Opening file: {}\n", filename);
//     }

//     auto ifs = std::ifstream{filename};

//     if (!ifs.is_open())
//     {
//         return 1;
//     }

//     const auto parsed_data = json::parse(ifs);

//     if (verbose)
//     {
//         const auto name = parsed_data["name"];
//         fmt::print("Name: {}\n", name);
//     }

//     return 0;
// }
