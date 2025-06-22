#include "jfif_parser.hpp"

#include <array>      // array
#include <bit>        // endian, byteswap
#include <fstream>    // ifstream
#include <ios>        // streampos, streamsize
#include <ostream>    // ostream
#include <stdexcept>  // runtime_error
#include <string>     // string
#include <utility>    // to_underlying

#include "common.hpp"

namespace lynx::jfif {

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

Parser::Parser(std::string file_name, const std::streampos seg_base) : common::Parser{file_name, seg_base} {
}

void jfif::Parser::Parse() {
    // Open the file
    std::ifstream ifs{file_name_, std::ifstream::binary};
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    // Go to the first byte of the JFIF Marker Segment
    ifs.seekg(seg_base_);

    // Skip the LENGTH field of the JFIF Marker Segment
    ifs.seekg(static_cast<std::streamoff>(common::Segment::FieldWidth::kLength), std::ios::cur);

    // Parse and skip the IDENTIFIER field of the JFIF Marker Segment
    std::array<char, std::to_underlying(Segment::FieldWidth::kIdentifier)> identifier{};
    ifs.read(identifier.data(), static_cast<std::streamsize>(Segment::FieldWidth::kIdentifier));
    if ((static_cast<std::uint8_t>(identifier[0]) != 0x4A) | (static_cast<std::uint8_t>(identifier[1]) != 0x46) | (static_cast<std::uint8_t>(identifier[2]) != 0x49) |
        (static_cast<std::uint8_t>(identifier[3]) != 0x46) | (static_cast<std::uint8_t>(identifier[4]) != 0x00)) {
        throw std::runtime_error("Bad JFIF Identifier");
    }

    /**********************/
    /* Parse JFIF Segment */
    /**********************/

    // Parse and skip the VERSION MAJOR field of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.version_major_), static_cast<std::streamsize>(Segment::FieldWidth::kVersionMajor));

    // Parse and skip the VERSION MINOR field of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.version_minor_), static_cast<std::streamsize>(Segment::FieldWidth::kVersionMajor));

    // Parse and skip the UNIT field of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.unit_), static_cast<std::streamsize>(Segment::FieldWidth::kUnit));

    // Parse and skip the XDENSITY field (always in big-endian) of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.x_density_), static_cast<std::streamsize>(Segment::FieldWidth::kXDensity));
    if (std::endian::native != std::endian::big) {
        seg_.x_density_ = std::byteswap(seg_.x_density_);
    }

    // Parse and skip the YDENSITY field (always in big-endian) of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.y_density_), static_cast<std::streamsize>(Segment::FieldWidth::kYDensity));
    if (std::endian::native != std::endian::big) {
        seg_.y_density_ = std::byteswap(seg_.y_density_);
    }

    // Parse and skip the XTHUMBNAIL field (always in big-endian) of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.x_thumbnail_), static_cast<std::streamsize>(Segment::FieldWidth::kXThumbnail));
    if (std::endian::native != std::endian::big) {
        seg_.x_thumbnail_ = std::byteswap(seg_.x_thumbnail_);
    }

    // Parse and skip the YTHUMBNAIL field (always in big-endian) of the JFIF Marker Segment
    ifs.read(reinterpret_cast<char *>(&seg_.y_thumbnail_), static_cast<std::streamsize>(Segment::FieldWidth::kYThumbnail));
    if (std::endian::native != std::endian::big) {
        seg_.y_thumbnail_ = std::byteswap(seg_.y_thumbnail_);
    }
}

}  // namespace lynx::jfif
