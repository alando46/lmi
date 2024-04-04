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
#include "tests/DoSomething.h"
#include <iostream>
#include <cassert>


int main() {

    json data = json::parse(R"(
{
  "whatToSay": "Hi I'm Bob!",
  "facialExpression": "grinning"
}
   )");

    DoSomething action = DoSomething.create(data);

    return 0;
}
