# Overview
| Directory |                 Comment                 |
| --------- | --------------------------------------- |
| `source`  | Library implementations                 |
| `include` | Library APIs                            |
| `example` | Demonstration on how to use the library |


# Building
In the project root directory, use one of the following commands to configure build type:
```bash
cmake -D CMAKE_BUILD_TYPE=Debug -B build .

cmake -D CMAKE_BUILD_TYPE=Release -B build .
```
In the project root directory, use the following commands to build:
```bash
cmake --build ./build
```


# Installing
In the `build` directory, use the following command to install the executable:
```bash
make install
```


# Running
In the `example` directory, use the following command to run the executable:
```bash
./demo <FILE_NAME_WITH_EXTENSION>
```


# Features
- Inheritance
- (Pure) virtual functions
- Constructor of derived classes
- Static cast
- Reinterpret cast
- Friend functions
- C++11: 
    - enum class
    - uniform initialization
    - fixed width integer types
    - smart pointers
- C++17: 
    - nested namespace
- C++20: 
    - `std::endian` from `<bit>`
- C++23: 
    - `std::byte_swap()` from `<bit>`
    - `std::to_underlying()` from `<utility>`