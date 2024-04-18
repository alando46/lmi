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
