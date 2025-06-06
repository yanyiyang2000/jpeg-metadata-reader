#ifndef LYNX_EXIF_PARSER_HPP
#define LYNX_EXIF_PARSER_HPP

#include <fstream>  // ifstream
#include <ios>      // streampos
#include <memory>   // unique_ptr
#include <ostream>  // ostream

#include "common.hpp"

namespace lynx::exif {

class Segment {
public:
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

}  // namespace lynx::exif

#endif  // LYNX_EXIF_PARSER_HPP
