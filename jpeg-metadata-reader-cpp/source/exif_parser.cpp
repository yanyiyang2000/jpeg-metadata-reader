#include "exif_parser.hpp"

#include <fstream>  // ifstream
#include <ios>      // streampos
#include <ostream>  // ostream

#include "common.hpp"

namespace lynx::exif {

Segment::Segment() {
}

Segment::~Segment() {
}

std::ostream& operator<<(std::ostream& os, const Segment& seg) {
    // TODO
    return os;
}

Parser::Parser(std::ifstream& ifs, std::streampos seg_base) : common::Parser{ifs, seg_base} {
}

Parser::~Parser() {
}

void Parser::Parse() {
}

}  // namespace lynx::exif