cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_COMPILER "/usr/bin/gcc")
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(Lynx LANGUAGES CXX)

########################################
# Use GCC and GCC standard C++ library #
########################################
add_compile_options(
    -std=c++23 # use C++23 standard
    -Wall      # enable all warnings
)

add_link_options(
    -lstdc++   # link GCC C++ standard library
)

##########################################
# Use Clang and GCC standard C++ library #
##########################################
# add_compile_options(
#     -std=c++23        # use C++23 standard
#     -stdlib=libstdc++ # use GCC C++ standard library
#     -Wall             # enable all warnings
# )

# add_link_options(
#     -lstdc++          # link GCC C++ standard library
# )

############################################
# Use Clang and Clang standard C++ library #
############################################
# add_compile_options(
#     -std=c++23     # use C++23 standard
#     -stdlib=libc++ # use Clang C++ standard library
#     -Wall          # enable all warnings
# )

# add_link_options(
#     -lc++          # link Clang C++ standard library
# )

add_subdirectory(${PROJECT_SOURCE_DIR}/source)
add_subdirectory(${PROJECT_SOURCE_DIR}/example)
