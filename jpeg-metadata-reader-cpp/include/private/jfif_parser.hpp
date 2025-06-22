#ifndef LYNX_JFIF_PARSER_HPP
#define LYNX_JFIF_PARSER_HPP

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t etc
#include <ios>      // streampos
#include <ostream>  // ostream
#include <string>   // string

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

    std::uint8_t  version_major_{0};
    std::uint8_t  version_minor_{0};
    std::uint8_t  unit_{0};
    std::uint16_t x_density_{0};
    std::uint16_t y_density_{0};
    std::uint8_t  x_thumbnail_{0};
    std::uint8_t  y_thumbnail_{0};

    /**
     * @brief Constructor
     *
     * @note This is an explicitly-defaulted constructor.
     */
    Segment() = default;

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    ~Segment() = default;

    friend std::ostream& operator<<(std::ostream& os, const Segment& seg);
};

/**
 * @brief JFIF Marker Segment parser
 */
class Parser : public common::Parser {
public:
    Segment seg_;

    /**
     * @brief Constructor
     *
     * @param `file_name` The name of the image file
     * @param `seg_base`  The offset of the first byte of the JFIF Marker Segment from the beginning of the file in bytes
     */
    Parser(std::string file_name, const std::streampos seg_base);

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    ~Parser() = default;

    /**
     * @brief Parses the JFIF Marker Segment.
     */
    virtual void Parse() override;
};

}  // namespace lynx::jfif

#endif  // LYNX_JFIF_PARSER_HPP
