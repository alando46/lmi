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

    std::string prompt = "Original prompt: You are a character in a fictitious game world. give a short greeting. Use the listed functions to make a response as a valid json";

    std::vector<std::unique_ptr<lmi::LMIFunction>> actions = lmi::makeOAIRequest<Action>("gpt-4-turbo", prompt, 1, 3);

    for (auto& action : actions) {
        Action* actionPtr = dynamic_cast<Action*>(action.get());
        if (actionPtr) {  // Check if the dynamic_cast was successful
            std::cout << "What to say: " << actionPtr->getWhatToSay() << std::endl;
            std::cout << "Facial expression: " << actionPtr->getFacialExpression() << std::endl;

        } else {
            std::cout << "Failed to cast LMIFunction pointer to Action pointer." << std::endl;
        }
    }
    return 0;
}
