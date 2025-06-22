#ifndef LYNX_COMMON_HPP
#define LYNX_COMMON_HPP

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t etc
#include <ios>      // streampos
#include <string>   // string

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
    const std::string    file_name_;
    const std::streampos seg_base_;

public:
    /**
     * @brief Constructor
     *
     * @param `file_name` The name of the image file
     * @param `seg_base`  The offset of the first byte of the Application Marker Segment from the beginning of the file in bytes
     */
    Parser(std::string file_name, const std::streampos seg_base);

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    virtual ~Parser() = default;

    /**
     * @brief Parses the Application Marker Segment
     *
     * @note This is a pure virtual function, hence the implementation depends on the derived classes.
     */
    virtual void Parse() = 0;
};

}  // namespace lynx::common

#endif  // LYNX_COMMON_HPP
