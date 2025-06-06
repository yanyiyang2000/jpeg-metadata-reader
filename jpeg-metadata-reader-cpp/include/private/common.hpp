#ifndef LYNX_COMMON_HPP
#define LYNX_COMMON_HPP

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t
#include <fstream>  // ifstream
#include <ios>      // streampos

namespace lynx::common {

class Segment {
public:
    enum class FieldWidth : std::size_t {
        kMarker = 2,
        kLength = 2
    };

    enum class Marker : std::uint8_t {
        kSOI  = 0xD8,
        kJfif = 0xE0,
        kExif = 0xE1,
    };
};

class Parser {
protected:
    std::ifstream& ifs_;
    std::streampos seg_base_;

public:
    Parser(std::ifstream& ifs, std::streampos seg_base);

    virtual ~Parser() = default;

    virtual void Parse() = 0;  // pure virtual function (implementation depends on derived classes)
};

}  // namespace lynx::common

#endif  // LYNX_COMMON_HPP
