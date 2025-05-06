cmake_minimum_required(VERSION 3.22)

# Uncomment to make cmake output verbose
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

project(JPEG_Reader LANGUAGES C)

add_compile_options(
    -Wall # enable all warnings
)

add_subdirectory(${PROJECT_SOURCE_DIR}/source)
add_subdirectory(${PROJECT_SOURCE_DIR}/example)
