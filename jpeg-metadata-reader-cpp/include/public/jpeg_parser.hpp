#ifndef LYNX_JPEG_PARSER_HPP
#define LYNX_JPEG_PARSER_HPP

#include <string>

namespace lynx::jpeg {

class Parser {
public:
    const std::string file_name_;

    Parser(std::string file_name);

    ~Parser();

    void Parse();
};

}  // namespace lynx::jpeg

#endif  // LYNX_JPEG_PARSER_HPP
