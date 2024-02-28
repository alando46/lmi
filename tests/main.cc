#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "validator.h"

class User : public JsonSchemaValidator {
    public:
        User(const nlohmann::json& j) : JsonSchemaValidator(j) {}
        void setupValidators() override;
};

TEST_CASE("User validator works on email" ) {

    SECTION("basic validation passes") {
        nlohmann::json j = R"({"name": "John Doe", "email": "john.doe@example.com"})"_json;
        User user(j);
        REQUIRE(user.isValid());
    }
    SECTION("Invalid object fails invalid email") {
        nlohmann::json j = R"({"name": "John Doe", "email": "emailatexample.com"})"_json;
        User user(j);
        REQUIRE(!user.isValid());
    }
    SECTION("Invalid object fails invalid email") {
        nlohmann::json j = R"({"name": "John Doe", "email": "e@mail@example.com"})"_json;
        User user(j);
        REQUIRE(!user.isValid());
    }
}

TEST_CASE("User validator works on name") {
    SECTION("Name valid length") {
    nlohmann::json j = R"({"name": "John Doe", "email": "john.doe@example.com"})"_json;
    User user(j);
    REQUIRE(user.isValid());
    }

    SECTION("Name too short") {
    nlohmann::json j = R"({"name": "Jd", "email": "john.doe@example.com"})"_json;
    User user(j);
    REQUIRE(!user.isValid());
    }
    SECTION("Name too long") {
    nlohmann::json j = R"({"name": "Jasdfgasdfgasdfgasdfgasdfgasdfgasdfgasdfgasdfgasdfg", "email": "john.doe@example.com"})"_json;
    User user(j);
    REQUIRE(!user.isValid());
    }
}
