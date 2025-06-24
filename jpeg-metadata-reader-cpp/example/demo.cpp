#include <memory>  // make_unique, unique_ptr

#include "jpeg_parser.hpp"

int main(int argc, char** argv) {
    // Option 1: static allocation
    lynx::jpeg::Parser  jpeg_parser{"example/example.jpg"};
    lynx::jpeg::Parser& jpeg_parser_ref{jpeg_parser};
    jpeg_parser_ref.Parse();

    // Option 2: dynamic allocation
    std::unique_ptr<lynx::jpeg::Parser> jpeg_parser_ptr_1{new lynx::jpeg::Parser{argv[1]}};
    jpeg_parser_ptr_1->Parse();

    // Option 3: another dynamic allocation
    std::unique_ptr jpeg_parser_ptr_2 = std::make_unique<lynx::jpeg::Parser>(argv[1]);
    jpeg_parser_ptr_2->Parse();

    return 0;
}
