#ifndef LYNX_EXIF_PARSER_HPP
#define LYNX_EXIF_PARSER_HPP

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t etc
#include <ios>      // streampos
#include <memory>   // unique_ptr
#include <mutex>    // mutex
#include <ostream>  // ostream
#include <string>   // string
#include <vector>   // vector

#include "common.hpp"

namespace lynx::exif {

/**
 * @brief Representation of Directory Entry (DE)
 */
class DirectoryEntry {
public:
    /**
     * @brief The data type of the fields of DE
     */
    struct FieldType {
        using Tag         = std::uint16_t;
        using ValueType   = std::uint16_t;
        using ValueCount  = std::uint32_t;
        using Values      = std::uint32_t;
        using ValueOffset = std::uint32_t;
        using IfdOffset   = std::uint32_t;
    };

    /**
     * @brief The width of the fields of DE in bytes
     */
    struct FieldWidth {
        static constexpr size_t Tag         = sizeof(FieldType::Tag);          // The width of TAG field in bytes
        static constexpr size_t ValueType   = sizeof(FieldType::ValueType);    // The width of VALUE TYPE field in bytes
        static constexpr size_t ValueCount  = sizeof(FieldType::ValueCount);   // The width of VALUE COUNT field in bytes
        static constexpr size_t Values      = sizeof(FieldType::Values);       // The width of VALUES/VALUE OFFSET/IFD OFFSET field in bytes
        static constexpr size_t ValueOffset = sizeof(FieldType::ValueOffset);  // The width of VALUES/VALUE OFFSET/IFD OFFSET field in bytes
        static constexpr size_t IfdOffset   = sizeof(FieldType::IfdOffset);    // The width of VALUES/VALUE OFFSET/IFD OFFSET field in bytes
    };

    enum class ValueTypeCode : std::size_t {
        kUInt8     = 1,
        kChar      = 2,
        kUInt16    = 3,
        kUInt32    = 4,
        kURational = 5,
        kInt8      = 6,
        kUndefined = 7,
        kInt16     = 8,
        kInt32     = 9,
        kRational  = 10,
        kSingle    = 11,
        kDouble    = 12
    };

    struct ValueType {
        using Byte           = std::uint8_t;   // 8-bit unsigned integer
        using SignedByte     = std::int8_t;    // 8-bit signed integer
        using Ascii          = char8_t;        // 8-bit byte that contains a 7-bit ASCII code; the last byte must be NULL
        using Undefined      = std::int8_t;    // 8-bit byte that may contain anything, depending on the definition of the field
        using Short          = std::uint16_t;  // 16-bit unsigned integer
        using SignedShort    = std::int16_t;   // 16-bit signed integer
        using Long           = std::uint32_t;  // 32-bit unsigned integer
        using SignedLong     = std::int32_t;   // 32-bit signed integer
        using Rational       = std::uint32_t;  // Two Long's: the first represents the numerator of a fraction, the second the denominator
        using SignedRational = std::int32_t;   // Two SignedLong's: the first represents the numerator of a fraction, the second the denominator
        using Float          = float;          // Single precision IEEE format
        using Double         = double;         // Double precision IEEE format
    };

    enum class ValueWidth : std::size_t {
        kUInt8     = 1,
        kInt8      = 1,
        kChar      = 1,
        kUndefined = 1,
        kUInt16    = 2,
        kInt16     = 2,
        kUInt32    = 4,
        kInt32     = 4,
        kSingle    = 4,
        kDouble    = 8,
        kURational = 16,
        kRational  = 16
    };

    std::uint16_t     tag_;
    std::size_t       value_type;
    std::size_t       value_count;
    std::vector<char> values_;

    /**
     * @brief Constructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    DirectoryEntry() = default;

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    virtual ~DirectoryEntry() = default;

    friend std::ostream& operator<<(std::ostream& os, const DirectoryEntry& de);
};

/**
 * @brief Representation of Image File Directory (IFD)
 */
class ImageFileDirectory {
public:
    /**
     * @brief The data type of the fields of IFD
     */
    struct FieldType {
        using DeCount   = std::uint16_t;
        using IfdOffset = std::uint32_t;
    };

    /**
     * @brief The width of the fields of IFD in bytes
     */
    struct FieldWidth {
        static constexpr size_t DeCount   = sizeof(FieldType::DeCount);    // The width of DE COUNT field in bytes
        static constexpr size_t De        = 12;                            // The width of a Directory Entry in bytes
        static constexpr size_t IfdOffset = sizeof(FieldType::IfdOffset);  // The width of IFD OFFSET field in bytes
    };

    std::vector<std::unique_ptr<DirectoryEntry>> des_;

    /**
     * @brief Constructor
     *
     * @note This is an explicitly-defaulted constructor.
     */
    ImageFileDirectory() = default;

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    ~ImageFileDirectory() = default;

    friend std::ostream& operator<<(std::ostream& os, const ImageFileDirectory& ifd);
};

/**
 * @brief Representation of Exif Marker Segment
 */
class Segment {
public:
    struct FieldType {
        using ByteOrder   = std::uint16_t;
        using MagicNumber = std::uint16_t;
        using IfdOffset   = std::uint32_t;
    };

    struct FieldWidth {
        static constexpr size_t Identifier  = 6;
        static constexpr size_t ByteOrder   = sizeof(FieldType::ByteOrder);
        static constexpr size_t MagicNumber = sizeof(FieldType::MagicNumber);
        static constexpr size_t IfdOffset   = sizeof(FieldType::IfdOffset);
    };

    std::endian                                      byte_order_;
    std::vector<std::unique_ptr<ImageFileDirectory>> ifds_;
    std::streampos                                   ifh_base_;

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
 * @brief Exif Marker Segment parser
 */
class Parser : public common::Parser {
private:
    static std::mutex cout_mutex_;  // tmp
    static std::mutex ifds_mutex_;

    /**
     * @brief Parses the IFD at the specified offset.
     *
     * @param `de_count`   The number of the DEs contained in the IFD
     * @param `ifd_offset` The offset of the IFD from the first byte of the IFH in bytes
     */
    void ParseIfd(const std::streampos ifd_offset);

public:
    Segment seg_;

    /**
     * @brief Constructor
     *
     * @param `file_name` The name of the image file
     * @param `seg_base`  The offset of the first byte of the Exif Marker Segment from the beginning of the file in bytes
     */
    Parser(std::string file_name, const std::streampos seg_base);

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    ~Parser() = default;

    /**
     * @brief Parses the Exif Marker Segment.
     */
    virtual void Parse() override;
};

}  // namespace lynx::exif

#endif  // LYNX_EXIF_PARSER_HPP
