# Prerequisites
Make sure the following commands are available:
- `make`
- `cmake`
- `gcc`
- `clang`

> [!NOTE]
> Sometimes `libc++` (Clang C++ standard library) and `libstdc++` (GCC C++ standard library) don't ship with the compiler, install them if necessary.


# Overview
| Directory |                 Comment                 |
| --------- | --------------------------------------- |
| `source`  | Library implementations                 |
| `include` | Library APIs                            |
| `example` | Demonstration on how to use the library |


# Building
In the project root directory, run the `build.sh` script to build the library and demo program.

> [!NOTE]
> Modify the `BUILD_TYPE` variable in the script for debug or release build. The default build type is `Debug`.


# Installing
The `build.sh` script will install the demo program in the `example` directory.


# Running
In the project root directory, use the following command to run the demo program:
```shell
example/demo
```


# Misc
Use the following commands to dump the predefined macros of GCC or Clang:
```shell
echo | gcc -std=c++23 -dM -E -x c++ -

echo | clang -std=c++23 -dM -E -x c++ -
```


# Features
- Inheritance
- (Pure) virtual functions
- Constructor of derived classes
- Static and cast
- Friend functions
- C++11:
    - enum class
    - uniform initialization
    - fixed width integer types
    - smart pointers
    - `default` keyword
    - `std::thread` from `<thread>`
    - `std::lock_guard` and `std::mutex` from `<mutex>`
    - `std::array` from `<array>`
- C++17:
    - nested namespace
- C++20:
    - `std::bit_cast` from `<bit>`
    - `std::endian` from `<bit>`
    - `std::span` from `<span>`
- C++23:
    - fixed width floating point types
    - `std::byte_swap()` from `<bit>`
    - `std::to_underlying()` from `<utility>`