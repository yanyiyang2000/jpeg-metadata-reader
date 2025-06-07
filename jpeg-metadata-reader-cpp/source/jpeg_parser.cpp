#include "jpeg_parser.hpp"

#include <bit>        // endian, byteswap
#include <cstdint>    // uint8_t. uint16_t
#include <cstring>    // memcpy
#include <fstream>    // ifstream
#include <ios>        // streamsize, streampos, streamoff
#include <iostream>   // cout
#include <ostream>    // flush
#include <stdexcept>  // runtime_error
#include <string>     // string
#include <utility>    // to_underlying
#include <vector>     // vector

#include "common.hpp"
#include "exif_parser.hpp"
#include "jfif_parser.hpp"

namespace lynx::jpeg {

Parser::Parser(std::string file_name) : file_name_{file_name} {
}

Parser::~Parser() {
}

void Parser::Parse() {
    std::ifstream ifs{file_name_, std::ifstream::binary};
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    std::vector<char> marker(std::to_underlying(common::Segment::FieldWidth::kMarker));

    // Parse and skip the MARKER field of SOI Marker Segment
    ifs.read(marker.data(), static_cast<std::streamsize>(common::Segment::FieldWidth::kMarker));
    if ((static_cast<std::uint8_t>(marker[0]) != 0xFF) | (static_cast<std::uint8_t>(marker[1]) != std::to_underlying(common::Segment::Marker::kSOI))) {
        throw std::runtime_error("SOI Marker Segment not found");
    }

    /*******************************/
    /* Process JFIF Marker Segment */
    /*******************************/

    // Parse and skip the MARKER field of JFIF Marker Segment
    ifs.read(marker.data(), static_cast<std::streamsize>(common::Segment::FieldWidth::kMarker));
    if ((static_cast<std::uint8_t>(marker[0]) != 0xFF) | (static_cast<std::uint8_t>(marker[1]) != std::to_underlying(common::Segment::Marker::kJfif))) {
        throw std::runtime_error("JFIF Marker Segment not found");
    }

    // Anchor the first byte of the LENGTH field of the JFIF Marker Segment
    std::streampos jfif_seg_base = ifs.tellg();

    // Parse and skip the LENGTH field (always in big-endian) of the JFIF Marker Segment
    std::uint16_t jfif_seg_len = 0;
    ifs.read(reinterpret_cast<char *>(&jfif_seg_len), static_cast<std::streamsize>(common::Segment::FieldWidth::kLength));
    if (std::endian::native != std::endian::big) {
        jfif_seg_len = std::byteswap(jfif_seg_len);
    }

    // Parse the JFIF Marker Segment
    jfif::Parser jfif_parser{ifs, jfif_seg_base};
    jfif_parser.Parse();

    // Print the JFIF Marker Segment
    std::cout << *jfif_parser.seg_ << std::flush;

    // Skip the rest of the JFIF Marker Segment
    ifs.seekg(jfif_seg_base + static_cast<std::streamoff>(jfif_seg_len));

    /*******************************/
    /* Process Exif Marker Segment */
    /*******************************/

    // Parse and skip the MARKER field of Exif Marker Segment
    ifs.read(marker.data(), static_cast<std::streamsize>(common::Segment::FieldWidth::kMarker));
    if ((static_cast<std::uint8_t>(marker[0]) != 0xFF) | (static_cast<std::uint8_t>(marker[1]) != std::to_underlying(common::Segment::Marker::kExif))) {
        throw std::runtime_error("Exif Marker Segment not found");
    }

    // Anchor the first byte of the LENGTH field of the Exif Marker Segment
    std::streampos exif_seg_base = ifs.tellg();

    // Parse and skip the LENGTH field (always in big-endian) of the Exif Marker Segment
    std::uint16_t exif_seg_len = 0;
    ifs.read(reinterpret_cast<char *>(&exif_seg_len), static_cast<std::streamsize>(common::Segment::FieldWidth::kLength));
    if (std::endian::native != std::endian::big) {
        exif_seg_len = std::byteswap(exif_seg_len);
    }

    // Parse the Exif Marker Segment
    exif::Parser exif_parser{ifs, exif_seg_base};
    exif_parser.Parse();

    // Print the Exif Marker Segment
    // TODO
    // std::cout << *exif_parser.seg_ << std::flush;

    // Skip the rest of the Exif Marker Segment
    ifs.seekg(exif_seg_base + static_cast<std::streamoff>(exif_seg_len));

    ifs.close();
}

}  // namespace lynx::jpeg
