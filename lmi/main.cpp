#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <variant>
#include <iostream>
#include "lmi/lmi.h"
#include "action.h"
#include <iostream>
#include <cassert>


int main() {

//     json data = json::parse(R"(
// {
//   "whatToSay": "Hi I'm Bob!",
//   "facialExpression": "laughing"
// }
//    )");

    json data = json::parse(R"(
{
    "function":{
        "arguments":"{\"facialExpression\":\"smiling\",\"whatToSay\":\"Hello, adventurer! Welcome to our mystical realm!\"}",
        "name":"TestWhatToSaySchema"},
        "id":"call_PcWSHzNnUgqde2K8IFgCvuGo",
        "type":"function"
}
    )");

    std::cout << data["function"]["arguments"] << std::endl;


    std::unique_ptr<Action> action_ptr = Action::create(data["function"]["arguments"]);

    // std::string prompt = "You are a character in a fictitious game world. give a short greeting.";

    // std::vector<std::unique_ptr<lmi::LMIFunction>> action = lmi::makeOAIRequest<Action>("gpt-4-turbo", prompt, 1, 3);

    return 0;
}
