#include "jfif_parser.hpp"

#include <bit>        // endian::native, endian::big, byteswap
#include <cstring>    // memcpy
#include <fstream>    // ifstream
#include <ios>        // streampos, streamsize
#include <memory>     // make_unique, move
#include <ostream>    // ostream, endl
#include <stdexcept>  // runtime_error
#include <utility>    // to_underlying
#include <vector>     // vector

#include "common.hpp"

namespace lynx::jfif {

Segment::Segment() {
}

Segment::~Segment() {
}

std::ostream &operator<<(std::ostream &os, const Segment &seg) {
    os << "Version Major:                    " << static_cast<unsigned int>(seg.version_major_) << '\n'
       << "Version Minor:                    " << static_cast<unsigned int>(seg.version_minor_) << '\n'
       << "Unit:                             " << static_cast<unsigned int>(seg.unit_) << '\n'
       << "Horizontal Pixel Density:         " << static_cast<unsigned int>(seg.x_density_) << '\n'
       << "Vertical Pixel Density:           " << static_cast<unsigned int>(seg.y_density_) << '\n'
       << "Thumbnail Horizontal Pixel Count: " << static_cast<unsigned int>(seg.x_thumbnail_) << '\n'
       << "Thumbnail Vertical Pixel Count:   " << static_cast<unsigned int>(seg.y_thumbnail_) << '\n';
    return os;
}

Parser::Parser(std::ifstream &ifs, std::streampos seg_base) : common::Parser{ifs, seg_base} {
}

Parser::~Parser() {
}

void jfif::Parser::Parse() {
    // Skip the LENGTH field
    ifs_.seekg(seg_base_ + static_cast<std::streamoff>(common::Segment::FieldWidth::kLength));

    // Parse and skip the IDENTIFIER field
    std::vector<char> buf(5);
    ifs_.read(buf.data(), static_cast<std::streamsize>(Segment::FieldWidth::kIdentifier));
    if ((static_cast<std::uint8_t>(buf[0]) != 0x4A) | (static_cast<std::uint8_t>(buf[1]) != 0x46) | (static_cast<std::uint8_t>(buf[2]) != 0x49) |
        (static_cast<std::uint8_t>(buf[3]) != 0x46) | (static_cast<std::uint8_t>(buf[4]) != 0x00)) {
        throw std::runtime_error("Bad JFIF Identifier");
    }

    /************************** */
    /* Construct a JFIF Segment */
    /************************** */

    std::unique_ptr seg = std::make_unique<Segment>();

    // Parse and skip the VERSION MAJOR field
    ifs_.read(reinterpret_cast<char *>(&seg->version_major_), static_cast<std::streamsize>(Segment::FieldWidth::kVersionMajor));

    // Parse and skip the VERSION MINOR field
    ifs_.read(reinterpret_cast<char *>(&seg->version_minor_), static_cast<std::streamsize>(Segment::FieldWidth::kVersionMajor));

    // Parse and skip the UNIT field
    ifs_.read(reinterpret_cast<char *>(&seg->unit_), static_cast<std::streamsize>(Segment::FieldWidth::kUnit));

    // Parse and skip the XDENSITY field (always in big-endian)
    ifs_.read(reinterpret_cast<char *>(&seg->x_density_), static_cast<std::streamsize>(Segment::FieldWidth::kXDensity));
    if (std::endian::native != std::endian::big) {
        seg->x_density_ = std::byteswap(seg->x_density_);
    }

    // Parse and skip the YDENSITY field (always in big-endian)
    ifs_.read(reinterpret_cast<char *>(&seg->y_density_), static_cast<std::streamsize>(Segment::FieldWidth::kYDensity));
    if (std::endian::native != std::endian::big) {
        seg->y_density_ = std::byteswap(seg->y_density_);
    }

    // Parse and skip the XTHUMBNAIL field (always in big-endian)
    ifs_.read(reinterpret_cast<char *>(&seg->x_thumbnail_), static_cast<std::streamsize>(Segment::FieldWidth::kXThumbnail));
    if (std::endian::native != std::endian::big) {
        seg->x_thumbnail_ = std::byteswap(seg->x_thumbnail_);
    }

    // Parse and skip the YTHUMBNAIL field (always in big-endian)
    ifs_.read(reinterpret_cast<char *>(&seg->y_thumbnail_), static_cast<std::streamsize>(Segment::FieldWidth::kYThumbnail));
    if (std::endian::native != std::endian::big) {
        seg->y_thumbnail_ = std::byteswap(seg->y_thumbnail_);
    }

    this->seg_ = std::move(seg);
}

}  // namespace lynx::jfif
