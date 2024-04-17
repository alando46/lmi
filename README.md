# Language Model Interface

LMI is an interface library for runtime validation and casting of JSON function calls to C++ objects.
The library uses [jsoncons](https://github.com/danielaparker/jsoncons) for the heavy lifting, and aims to provide
a mix of functionalities similar to Python's [Pydantic](https://github.com/pydantic/pydantic) and [Instructor](https://github.com/jxnl/instructor) libraries.

## Example workflow

1. Create desired child class.

```C++
class MoveTo : public lmi::LMIFunction {


}
```

2. Add required private attributes `rawJson_` and `jsonSchema_`.

```C++
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
    // data attribs
    MovementStyle movementStyle_;
    Destination destination_;

protected:

    void setValues() {
        movementStyle_ = validStrToMovStyle(rawJson_["movementStyle"].as<std::string>());
        destination_ = validStrToDest(rawJson_["destination"].as<std::string>());
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
6. 
```C++
#include "moveTo.h"

jsoncons::json Action::jsonSchema_ = json::parse(R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "MoveToSchema",
    "description": "A basic schema for the movement function",
    "type": "object",
    "properties": {
        "destination": {
            "description": "The destination the character should move to",
            "enum": [ "smiling", "thinking", "serious" ]
            "maxLength": 5
        },
        "facialExpression": {
            "description": "The facial expression the character should have",
            "enum": [ "smiling", "thinking", "serious" ]
        }
    },
    "required": [ "whatToSay", "facialExpression" ]
}
)");

```


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
