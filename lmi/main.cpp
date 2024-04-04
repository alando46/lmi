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
#include <iostream>
#include <cassert>


int main() {

    json data2 = json::parse(R"(
{
  "whatToSay": "Hi I'm Bob!"
}
   )");

    class SaySomething : public lmi::LMIFunction {
    private:
        std::string whatToSay_;
        jsoncons::json jsonSchema_ = json::parse(R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "TestWhatToSaySchema",
    "description": "A basic schema for testing",
    "type": "object",
    "properties": {
        "whatToSay": {
            "description": "The dialog that should be said",
            "type": "string",
            "minLength": 1,
            "maxLength": 140
        }
    },
    "required": [ "whatToSay" ]
}
        )");
    protected:
        void setValues() override {
            whatToSay_ = rawJson_["whatToSay"].as<std::string>();
        }
    public:
        SaySomething(jsoncons::json rawJson) : lmi::LMIFunction(rawJson) {
            validateAndSet();
        }

        std::string getWhatToSay() {
            return whatToSay_;
        }
    };

    SaySomething something(data2);

    return 0;
}
