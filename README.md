# Language Model Interface

LMI is an interface library for runtime validation and casting of JSON function calls to C++ objects.
The library uses [jsoncons](https://github.com/danielaparker/jsoncons) for the heavy lifting, and aims to provide
a mix of functionalities similar to Python's [Pydantic](https://github.com/pydantic/pydantic) and [Instructor](https://github.com/jxnl/instructor) libraries.

## Example workflow

1. 
```C++

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
