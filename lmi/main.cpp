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

    json data = json::parse(R"(
{
  "whatToSay": "Hi I'm Bob!",
  "facialExpression": "laughing"
}
   )");

    Action action = Action::create(data);

    // how to return multiple validation errors at the same time?
    // how does pydantic handle this?
    // what is the most c++ way to handle this?

    return 0;
}
