// #define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <jsoncons/json.hpp>
#include <string>

#include "lmi/lmi.h"

TEST_CASE("JSON->C++->JSON NO VALIDATION")

    std::ifstream inputFile("data/openAIResponse.json");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    jsoncons::json testJson;
    try {
        testJson = json::parse(inputFile);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;

    // convert json to ChatCompletion object
    lmi::openAI::ChatCompletion cc = testJson.as<lmi::openAI::ChatCompletion>();
    // convert back to json and render as string
    std::string jsonString = jsoncons::encode_json(cc);
    REQUIRE(jsonString == testJson.to_string())


// class User : public JsonSchemaValidator {
//     public:
//         User(const nlohmann::json& j) : JsonSchemaValidator(j) {}
//         void setupValidators() override {
//             validators["name"] = std::make_shared<LengthValidator>(3, 15);
//             validators["email"] = std::make_shared<RegexValidator>("^(\\w+)(\\.\\w+)*@(\\w+)(\\.(\\w+))+");
//         }
// };


// class BaseClass {
// public:
//     template<typename T, typename Func>
//     requires std::predicate<Func, T>
//     auto Field(const T& value, const Func& validator, std::string repr) {
//         if (!validator(value)) {
//             throw ValidationError(repr, "Validation failed for the provided value: ");
//         }
//         obj[repr] = value;
//         return value;
//     }
// private:
//     nlohmann::json obj;
// };

// class User2 : public BaseClass {
//     public:
//         std::string name;
//         int age;
//         float weight;

//         User2(std::string name, int age, float weight) {
//             this->name = Field(name, validateName, "name");
//             this->age = Field(age, validateAge, "age");
//             this->weight = Field(weight, validateWeight, "weight");
//         }

//         static bool validateName(std::string name) {
//             return !name.empty();
//         }

//         static bool validateAge(int age) {
//             return age > 0 && age < 100;
//         }

//         static bool validateWeight(float weight) {
//             return weight > 0;
//         }
// };


// TEST_CASE("Validator MVP") {

//     SECTION("Basic Validation") {
//         User2 user2("bill", 10, 90.0);
//         REQUIRE(user2.name == "bill");
//         REQUIRE(user2.age == 10);
//         REQUIRE(user2.weight == 90);
//     }

//     SECTION("Invalid name") {
//         REQUIRE_THROWS_WITH(User2("", 10, 90.0), Catch::Matchers::Contains("Validation failed for the provided value: name"));
//     }
// }

// TEST_CASE("User validator works on email" ) {

//     SECTION("basic validation passes") {
//         nlohmann::json j = R"({"name": "John Doe", "email": "john.doe@example.com"})"_json;
//         User user(j);
//         REQUIRE(user.isValid());
//     }
//     SECTION("Invalid object fails invalid email") {
//         nlohmann::json j = R"({"name": "John Doe", "email": "emailatexample.com"})"_json;
//         User user(j);
//         REQUIRE(!user.isValid());
//     }
//     SECTION("Invalid object fails invalid email") {
//         nlohmann::json j = R"({"name": "John Doe", "email": "e@mail@example.com"})"_json;
//         User user(j);
//         REQUIRE(!user.isValid());
//     }
// }

// TEST_CASE("User validator works on name") {
//     SECTION("Name valid length") {
//     nlohmann::json j = R"({"name": "John Doe", "email": "john.doe@example.com"})"_json;
//     User user(j);
//     REQUIRE(user.isValid());
//     }

//     SECTION("Name too short") {
//     nlohmann::json j = R"({"name": "Jd", "email": "john.doe@example.com"})"_json;
//     User user(j);
//     REQUIRE(!user.isValid());
//     }
//     SECTION("Name too long") {
//     nlohmann::json j = R"({"name": "Jasdfgasdfgasdfgasdfgasdfgasdfgasdfgasdfgasdfgasdfg", "email": "john.doe@example.com"})"_json;
//     User user(j);
//     REQUIRE(!user.isValid());
//     }
// }
