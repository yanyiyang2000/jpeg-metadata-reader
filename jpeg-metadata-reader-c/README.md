# Prerequisites
Make sure the following commands are available:
- `make`
- `cmake`
- `gcc`


# Overview
| Directory |                 Comment                 |
| --------- | --------------------------------------- |
| `source`  | Library implementations                 |
| `include` | Library APIs                            |
| `example` | Demonstration on how to use the library |


# Building
In the project root directory, use one of the following commands to configure build type:
```bash
cmake -D CMAKE_BUILD_TYPE=Debug -B ./build .

cmake -D CMAKE_BUILD_TYPE=Release -B ./build .
```
In the project root directory, use the following commands to build:
```bash
cmake --build ./build
```


# Installing
In the project root directory, use the following command to install the executable:
```bash
make -C build install
```


# Running
In the `example` directory, use the following command to run the executable:
```bash
./demo <FILE_NAME_WITH_EXTENSION>
```
