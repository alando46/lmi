# Language Model Interface

LMI is an interface library for runtime validation and casting of JSON function calls to C++ objects.
The library uses [jsoncons](https://github.com/danielaparker/jsoncons) for the heavy lifting, and aims to provide
a mix of functionalities similar to Python's [Pydantic](https://github.com/pydantic/pydantic) and [Instructor](https://github.com/jxnl/instructor) libraries.

## Example workflow

The following tutorial covers the basics of how to create a `LMIFunction` subclass, and include
it as a response model in a chat completion request. 

1. Create desired child class. In this case, we will crfeate a class that moves a NPC to a new location 
based on a LLM function call.

```C++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>
#include <string>
#include "lmi/lmi.h"

class MoveTo : public lmi::LMIFunction {
}
```

2. Add required private attributes `rawJson_` and `jsonSchema_`.

```C++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>
#include <string>
#include "lmi/lmi.h"

class MoveTo : public lmi::LMIFunction {
private:
    // rawJson_ is required
    jsoncons::json rawJson_;
    // jsonSchema_ is required
    // (static is required so can be used by constructor)
    static jsoncons:: jsonSchema_;
}
```

3. Define data attributes and any conversion necessary for setting them. Setting and conversion happens in
`setValues()`

```C++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>
#include <string>
#include "lmi/lmi.h"

enum class MovementStyle {
    WalkCalm,
    WalkFast,
    Jog,
    FranticSprint,
    Default
}

enum class Destination {
    GroceryStore1,
    BillHome,
    BillWork,
    SueHome,
    CurrentLocation,
}

MovementStyle validStrToMovStyle(const std::string& rawStr) {
    if (rawStr == "WalkCalm") {
        return MovementStyle::WalkCalm;
    }
    else if (rawStr == "WalkFast") {
        return MovementStyle::WalkFast;
    }
    else if (rawStr == "Jog") {
        return MovementStyle::Jog;
    }
    else if (rawStr == "FranticSprint") {
        return MovementStyle::FranticSprint;
    }
    else {
        return MovementStyle::Default;
    }
}

Destination validStrToDest(const std::string& rawStr) {
    if (rawStr == "GroceryStore1") {
        return Desination::GroceryStore1;
    }
    else if (rawStr == "BillHome") {
        return Destination::BillHome;
    }
    else if (rawStr == "BillWork") {
        return Destination::BillWork;
    }
    else if (rawStr == "SueHome") {
        return Detination::SueHome;
    }
    else {
        return Destination::CurrentLocation;
    }
}

class MoveTo: public lmi::Function {
private:
    jsoncons::json rawJson_;
    static jsoncons:: jsonSchema_;

    // data attribs
    MovementStyle movementStyle_;
    Destination destination_;

protected:
    // required, must define this method and set data attribs from
    // raw json
    void setValues() {
        movementStyle_ = validStrToMovStyle(rawJson_["movementStyle"].as<std::string>());
        destination_ = validStrToDest(rawJson_["destination"].as<std::string>());
    }

public:
    // required, must return schema
    jsoncons::json getSchema() {
        return jsonSchema_;
    }
}
```

4. Call `setValues()` in constructor.

```C++
class MoveTo : public lmi::LMIFunction {
private:
    jsoncons::json rawJson_;
    static jsoncons:: jsonSchema_;
public:
    Action(jsoncons::json rawJson) : rawJson_(rawJson){
        // required, must call here
        setValues();
    }
}
```

5. In the implementation file, `moveTo.cpp`, define a jsoncons json which holds schema for validation.

```C++
#include "moveTo.h"

jsoncons::json Action::jsonSchema_ = json::parse(R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "MoveToSchema",
    "description": "A basic schema for the moveTo function",
    "type": "object",
    "properties": {
        "destination": {
            "description": "The destination the character should move to",
            "enum": [ "GroceryStore1", "BillHome", "BillWork", "SueHome", "CurrentLocation" ]
            "maxLength": 5
        },
        "moveStyle": {
            "description": "The style of movement the character should adhere to while moving",
            "enum": [ "WalkCalm", "WalkFast", "Jog", "FranticSprint", "Default" ]
        }
    },
    "required": [ "destination", "moveStyle" ]
}
)");
```

TODO: show how how to call makeOAIRequest with the `MoveTo` class created above, and get an instantiated instance of `MoveTo` as output.




## Documentation for building the project

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release --target main
cd app
./main
```

- Unit testing

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target unit_tests
cd tests
./unit_tests
```

- Documentation

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target docs
```

- Code Coverage (Unix only)

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON ..
cmake --build . --config Debug --target coverage
```

For more info about CMake see [here](./README_cmake.md).
