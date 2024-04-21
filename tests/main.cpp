// #define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
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
#include "lmi/client.h"
#include "DoSomething.h"


TEST_CASE("Instantiating LMIFunction subclass and validation workflow works for invalid data - multiple fields") {
    json data = json::parse(R"(
{
  "whatToSay": "Hi I'm Bob!",
  "facialExpression": "grinning"
}
   )");

    REQUIRE_THROWS_WITH(Action::create(data), Catch::Matchers::ContainsSubstring("grinning is not a valid enum value"));
    REQUIRE_THROWS_WITH(Action::create(data), Catch::Matchers::ContainsSubstring("Expected maxLength: 5, actual: 11"));
}


// TEST_CASE("Instantiating LMIFunction subclass works with OAI data") {

//     json data = json::parse(R"(
// {
//     "function":{
//         "arguments":"{\"facialExpression\":\"smiling\",\"whatToSay\":\"Hello, adventurer! Welcome to our mystical realm!\"}",
//         "name":"TestWhatToSaySchema"},
//         "id":"call_PcWSHzNnUgqde2K8IFgCvuGo",
//         "type":"function"
// }
//     )");

// }

TEST_CASE( "Validation schema transform returns expected values - case 1" ) {

    jsoncons::json testValidationSchema1 = json::parse(R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "TestWhatToSaySchema",
    "description": "A basic schema for testing",
    "type": "object",
    "properties": {
        "whatToSay": {
            "description": "The dialog that should be said",
            "type": "string",
            "maxLength": 40
        },
        "facialExpression": {
            "description": "The facial expression the character should have",
            "enum": [ "smiling", "thinking", "serious" ]
        }
    },
    "required": [ "whatToSay", "facialExpression" ]
}
    )");

    SECTION( "transform to OpenAI Schema works" ) {
    jsoncons::json openAITransformedSchema1 = json::parse(R"(
{
    "type": "function",
    "function": {
        "name": "TestWhatToSaySchema",
        "description": "A basic schema for testing",
        "parameters" : {
            "type": "object",
            "properties": {
                "whatToSay": {
                    "description": "The dialog that should be said",
                    "type": "string",
                    "maxLength": 40
                },
                "facialExpression": {
                    "description": "The facial expression the character should have",
                    "enum": [ "smiling", "thinking", "serious" ]
                }
            },
            "required": [ "whatToSay", "facialExpression" ]
        }
    }
}
    )");
    REQUIRE(lmi::openAISchemaTransform(testValidationSchema1).as<std::string>() == openAITransformedSchema1.as<std::string>());
    }
    // Add additional sections for Anthropic schema, etc...
}
