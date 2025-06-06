#ifndef LYNX_JFIF_PARSER_HPP
#define LYNX_JFIF_PARSER_HPP

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t, uint16_t
#include <fstream>  // ifstream
#include <ios>      // streampos
#include <memory>   // unique_ptr
#include <ostream>  // ostream

#include "common.hpp"

namespace lynx::jfif {

class Segment {
public:
    enum class FieldWidth : std::size_t {
        kIdentifier   = 5,
        kVersionMajor = 1,
        kVersionMinor = 1,
        kUnit         = 1,
        kXDensity     = 2,
        kYDensity     = 2,
        kXThumbnail   = 1,
        kYThumbnail   = 1
    };

    std::uint8_t  version_major_;
    std::uint8_t  version_minor_;
    std::uint8_t  unit_;
    std::uint16_t x_density_;
    std::uint16_t y_density_;
    std::uint8_t  x_thumbnail_;
    std::uint8_t  y_thumbnail_;

    Segment();

    ~Segment();

    friend std::ostream& operator<<(std::ostream& os, const Segment& seg);
};

class Parser : public common::Parser {
public:
    std::unique_ptr<Segment> seg_;

    Parser(std::ifstream& ifs, std::streampos seg_base);

    ~Parser();

    virtual void Parse();
};

}  // namespace lynx::jfif

#endif  // LYNX_JFIF_PARSER_HPP
