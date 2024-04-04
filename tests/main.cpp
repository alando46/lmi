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

// namespace testNS {

//     struct Vegetable {
//         std::string veggieName;
//         bool yummy;
//     };

//     // schema class
//     class Produce: public lmi::SchemaBase<Produce> {
//     public:
//         std::vector<std::string> fruits;
//         std::vector<Vegetable> vegetables;
//     };
// } // testNS

// JSONCONS_ALL_MEMBER_TRAITS(testNS::Vegetable, veggieName, yummy)
// JSONCONS_ALL_MEMBER_TRAITS(testNS::Produce, fruits, vegetables)

// TEST_CASE("Encoding and decoding json to/from subclassed schema works with valid data") {
//     json data = json::parse(R"(
// {
//   "fruits": [ "grapefruit", "orange", "banana" ],
//   "vegetables": [
//     {
//       "veggieName": "potato",
//       "yummy": true
//     },
//     {
//       "veggieName": "broccoli",
//       "yummy": true
//     },
//     {
//       "veggieName": "carrot",
//       "yummy": false
//     },
//     {
//       "veggieName": "Swiss Chard",
//       "yummy": true
//     }
//   ]
// }
//    )");
//     testNS::Produce p = data.as<testNS::Produce>();
//     REQUIRE(p.fruits == data["fruits"]);
//     REQUIRE(p.vegetables == data["vegetables"]);
//     std::string testResultsJsonStr;
//     jsoncons::encode_json(p, testResultsJsonStr);
//     REQUIRE(testResultsJsonStr == data.as_string());
// }

// TEST_CASE("Instantiating LMIFunction subclass and validation workflow works for valid data") {
//     json data = json::parse(R"(
// {
//   "whatToSay": "Hi I'm Bob!"
// }
//    )");

//     class SaySomething : public lmi::LMIFunction {
//     private:
//         std::string whatToSay_;
//         // must define
//         jsoncons::json jsonSchema_ = json::parse(R"(
// {
//     "$schema": "http://json-schema.org/draft-07/schema#",
//     "title": "TestWhatToSaySchema",
//     "description": "A basic schema for testing",
//     "type": "object",
//     "properties": {
//         "whatToSay": {
//             "description": "The dialog that should be said",
//             "type": "string",
//             "minLength": 1,
//             "maxLength": 140
//         }
//     },
//     "required": [ "whatToSay" ]
// }
//         )");
//     protected:
//         // must implement
//         void setValues() override {
//             whatToSay_ = rawJson_["whatToSay"].as<std::string>();
//         }
//     public:
//         SaySomething(jsoncons::json rawJson) : lmi::LMIFunction(rawJson) {
//             // must call this here, because if we validate and set in the parent
//             // constructor, jsonSchema_ will still be null because this object is not initialized yet
//             // alternatively we could define the validation schema within this construtor block,
//             // but that seems worse
//             validateAndSet();
//         }

//         std::string getWhatToSay() {
//             return whatToSay_;
//         }
//     };

//     SaySomething something(data);

//     REQUIRE(something.getWhatToSay() == data["whatToSay"]);
// }


// TEST_CASE("Instantiating LMIFunction subclass and validation workflow works for invalid data") {
//     json data = json::parse(R"(
// {
//   "whatToSayyy": "Hi I'm Bob!"
// }
//    )");

//     class SaySomething : public lmi::LMIFunction {
//     private:
//         std::string whatToSay_;
//         // must define
//         static std::string jsonSchema_ = R"(
// {
//     "$schema": "http://json-schema.org/draft-07/schema#",
//     "title": "TestWhatToSaySchema",
//     "description": "A basic schema for testing",
//     "type": "object",
//     "properties": {
//         "whatToSay": {
//             "description": "The dialog that should be said",
//             "type": "string",
//             "minLength": 1,
//             "maxLength": 140
//         }
//     },
//     "required": [ "whatToSay" ]
// }
//         )";
//     protected:
//         // must implement
//         void setValues() override {
//             whatToSay_ = rawJson_["whatToSay"].as<std::string>();
//         }
//     public:
//         SaySomething(jsoncons::json rawJson) : lmi::LMIFunction(rawJson) {
//             // must call this here, because if we validate and set in the parent
//             // constructor, jsonSchema_ will still be null because this object is not initialized yet
//             // alternatively we could define the validation schema within this construtor block,
//             // but that seems worse
//             validateAndSet();
//         }

//         std::string getWhatToSay() {
//             return whatToSay_;
//         }
//     };

//     REQUIRE_THROWS_WITH(SaySomething(data), Catch::Matchers::ContainsSubstring("Key not found: 'whatToSay'"));
// }


// TEST_CASE("Instantiating LMIFunction subclass and validation workflow works for invalid data - multiple fields") {
//     json data = json::parse(R"(
// {
//   "whatToSayyy": "Hi I'm Bob!",
//   "facialExpression": "grinning"
// }
//    )");

//     class SaySomething : public lmi::LMIFunction {
//     private:
//         std::string whatToSay_;
//         std::string facialExpression_;
//         // must define
//         jsoncons::json jsonSchema_ = json::parse(R"(
// {
//     "$schema": "http://json-schema.org/draft-07/schema#",
//     "title": "TestWhatToSaySchema",
//     "description": "A basic schema for testing",
//     "type": "object",
//     "properties": {
//         "whatToSay": {
//             "description": "The dialog that should be said",
//             "type": "string",
//             "minLength": 1,
//             "maxLength": 140
//         },
//         "facialExpression": {
//             "description": "The facial expression the character should have",
//             "enum": ["smiling", "thinking", "serious"]
//         }
//     },
//     "required": [ "whatToSay", "facialExpression" ]
// }
//         )");
//     protected:
//         // must implement
//         void setValues() override {
//             whatToSay_ = rawJson_["whatToSay"].as<std::string>();
//             facialExpression_ = rawJson_["facialExpression"].as<std::string>();
//         }
//     public:
//         SaySomething(jsoncons::json rawJson) : lmi::LMIFunction(rawJson) {
//             // must call this here, because if we validate and set in the parent
//             // constructor, jsonSchema_ will still be null because this object is not initialized yet
//             // alternatively we could define the validation schema within this construtor block,
//             // but that seems worse
//             validateAndSet();
//         }

//         std::string getWhatToSay() {
//             return whatToSay_;
//         }

//         std::string getFacialExpression() {
//             return facialExpression_;
//         }
//     };

//     REQUIRE_THROWS_WITH(SaySomething(data), Catch::Matchers::ContainsSubstring("Key not found: 'whatToSay'"));
// }
