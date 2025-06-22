#include "exif_parser.hpp"

#include <array>     // array
#include <bit>       // bit_cast, endian, byteswap
#include <cstddef>   // size_t
#include <cstring>   // memcpy
#include <fstream>   // ifstream
#include <iomanip>   // setfill, setw
#include <ios>       // streampos, streamoff
#include <iostream>  // cout
#include <memory>    // unique_ptr, make_unique
#include <mutex>     // lock_guard, mutex
#include <ostream>   // ostream
#include <span>      // span
#include <string>    // string
#include <thread>    // thread
#include <utility>   // to_underlying
#include <vector>    // vector

#include "common.hpp"

namespace lynx::exif {

// std::mutex Parser::cout_mutex_;
std::mutex Parser::ifds_mutex_;

std::ostream& operator<<(std::ostream& os, const Segment& seg) {
    for (const auto& ifd : seg.ifds_) {
        for (const auto& de : ifd->des_) {
            os << *de << "\n";
        }
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const DirectoryEntry& de) {
    os << "Tag: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << de.tag_ << std::dec << " Type: " << std::setfill(' ') << std::setw(2) << de.value_type
       << " Count: " << std::setfill(' ') << std::setw(4) << de.value_count << " Values: ";

    std::span<const char> values(de.values_);
    switch (de.value_type) {
            // case (std::to_underlying(DirectoryEntry::ValueTypeCode::kUInt16)): {
            //     for (size_t i = 0; i < de.value_count; i++) {
            //         std::array<char, 2> value = {values[2*i], values[2*i + 1]};
            //         os << std::bit_cast<uint16_t>(value) << " ";
            //     }
            //     break;
            // }

        case (std::to_underlying(DirectoryEntry::ValueTypeCode::kChar)): {
            for (size_t i = 0; i < de.value_count; i++) {
                os << values[i];
            }
            break;
        }

            // case (std::to_underlying(DirectoryEntry::ValueTypeCode::kUInt32)): {
            //     for (size_t i = 0; i < de.value_count; i++) {
            //         std::array<char, 4> value = {values[4*i], values[4*i + 1], values[4*i + 2], values[4*i + 3]};
            //         os << std::bit_cast<uint32_t>(value) << " ";
            //     }
            //     break;
            // }
    }

    return os;
}

Parser::Parser(std::string file_name, const std::streampos seg_base) : common::Parser{file_name, seg_base} {
}

void Parser::Parse() {
    // Open the image file
    std::ifstream ifs{file_name_, std::ifstream::binary};
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    // Go to the first byte of the Exif Marker Segment
    ifs.seekg(seg_base_);

    // Skip the LENGTH field of the Exif Marker Segment
    ifs.seekg(static_cast<std::streamoff>(common::Segment::FieldWidth::kLength), std::ios::cur);

    // Parse and skip the IDENTIFIER field of the Exif Marker Segment
    std::array<char, Segment::FieldWidth::Identifier> identifier_buf{};
    ifs.read(identifier_buf.data(), identifier_buf.size());
    if ((identifier_buf[0] != 0x45) | (identifier_buf[1] != 0x78) | (identifier_buf[2] != 0x69) | (identifier_buf[3] != 0x66) | (identifier_buf[4] != 0x00) |
        (identifier_buf[5] != 0x00)) {
        throw std::runtime_error("Bad Exif Identifier");
    }

    /*********************************/
    /* Parse Image File Header (IFH) */
    /*********************************/

    // Anchor the first byte of the IFH
    seg_.ifh_base_ = ifs.tellg();

    // Parse and skip the BYTE ORDER field of the IFH
    std::array<char, Segment::FieldWidth::ByteOrder> byte_order_buf{};
    ifs.read(byte_order_buf.data(), byte_order_buf.size());
    auto byte_order = std::bit_cast<Segment::FieldType::ByteOrder>(byte_order_buf);
    switch (byte_order) {
        case 0x4D4D: {
            seg_.byte_order_ = std::endian::big;
            break;
        }

        case 0x4949: {
            seg_.byte_order_ = std::endian::little;
            break;
        }

        default: {
            throw std::runtime_error("Bad Byte Order");
        }
    }

    // Parse and skip the MAGIC NUMBER field of the IFH
    std::array<char, Segment::FieldWidth::MagicNumber> magic_number_buf{};
    ifs.read(magic_number_buf.data(), magic_number_buf.size());
    auto magic_number = std::bit_cast<Segment::FieldType::MagicNumber>(magic_number_buf);
    if (seg_.byte_order_ != std::endian::native) {
        magic_number = std::byteswap(magic_number);
    }
    if (magic_number != 0x002A) {
        throw std::runtime_error("Bad Magic Number");
    }

    // Parse and skip the OFFSET field of the IFH
    std::array<char, Segment::FieldWidth::IfdOffset> ifd_offset_buf{};
    ifs.read(ifd_offset_buf.data(), ifd_offset_buf.size());
    auto ifd_offset = std::bit_cast<Segment::FieldType::IfdOffset>(ifd_offset_buf);
    if (std::endian::native != seg_.byte_order_) {
        ifd_offset = std::byteswap(ifd_offset);
    }

    /***************************************/
    /* Parse Image File Directories (IFDs) */
    /***************************************/

    std::vector<std::thread> threads{};

    // There exists more IFDs
    while (ifd_offset != 0) {
        // Go to the first byte of the IFD
        ifs.seekg(seg_.ifh_base_ + static_cast<std::streampos>(ifd_offset));

        // Parse and skip the DE COUNT field of the IFD
        std::array<char, ImageFileDirectory::FieldWidth::DeCount> de_count_buf{};
        ifs.read(de_count_buf.data(), de_count_buf.size());
        auto de_count = std::bit_cast<ImageFileDirectory::FieldType::DeCount>(de_count_buf);
        if (std::endian::native != seg_.byte_order_) {
            de_count = std::byteswap(de_count);
        }

        // Spawn a thread to parse the IFD
        threads.emplace_back(&Parser::ParseIfd, this, ifd_offset);

        // Skip the DEs
        ifs.seekg(static_cast<std::streamoff>(ImageFileDirectory::FieldWidth::De * de_count), std::ios::cur);

        // Parse and skip the OFFSET field of the IFD
        ifs.read(ifd_offset_buf.data(), ifd_offset_buf.size());
        ifd_offset = std::bit_cast<ImageFileDirectory::FieldType::IfdOffset>(ifd_offset_buf);
        if (std::endian::native != seg_.byte_order_) {
            ifd_offset = std::byteswap(ifd_offset);
        }
    }

    // Join the threads
    for (auto& t : threads) {
        t.join();
    }
}

void Parser::ParseIfd(const std::streampos ifd_offset) {
    // Open the image file
    std::ifstream ifs{file_name_, std::ifstream::binary};
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    // Go to the first byte of the first IFD
    ifs.seekg(seg_.ifh_base_ + ifd_offset);

    // Parse and skip the DE COUNT field of the IFD
    std::array<char, ImageFileDirectory::FieldWidth::DeCount> de_count_buf{};
    ifs.read(de_count_buf.data(), de_count_buf.size());
    auto de_count = std::bit_cast<ImageFileDirectory::FieldType::DeCount>(de_count_buf);
    if (std::endian::native != seg_.byte_order_) {
        de_count = std::byteswap(de_count);
    }

    /*********************************/
    /* Parse Directory Entries (DEs) */
    /*********************************/

    std::unique_ptr ifd_ptr{std::make_unique<ImageFileDirectory>()};

    for (size_t i = 0; i < de_count; i++) {
        // Parse and skip the TAG field of the DE
        std::array<char, DirectoryEntry::FieldWidth::Tag> tag_buf{};
        ifs.read(tag_buf.data(), tag_buf.size());
        auto tag = std::bit_cast<DirectoryEntry::FieldType::Tag>(tag_buf);
        if (std::endian::native != seg_.byte_order_) {
            tag = std::byteswap(tag);
        }

        // Parse and skip the VALUE TYPE field of the DE
        std::array<char, DirectoryEntry::FieldWidth::ValueType> value_type_buf{};
        ifs.read(value_type_buf.data(), value_type_buf.size());
        auto value_type = std::bit_cast<DirectoryEntry::FieldType::ValueType>(value_type_buf);
        if (std::endian::native != seg_.byte_order_) {
            value_type = std::byteswap(value_type);
        }

        // Parse and skip the VALUE COUNT field of the DE
        std::array<char, DirectoryEntry::FieldWidth::ValueCount> value_count_buf{};
        ifs.read(value_count_buf.data(), value_count_buf.size());
        auto value_count = std::bit_cast<DirectoryEntry::FieldType::ValueCount>(value_count_buf);
        if (std::endian::native != seg_.byte_order_) {
            value_count = std::byteswap(value_count);
        }

        // Determine if the VALUE OFFSET field contains the actual values or the offset to the actual values
        std::unique_ptr       de_ptr{std::make_unique<DirectoryEntry>()};
        std::size_t           value_size{0};
        constexpr std::size_t value_capacity{4};
        switch (value_type) {
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kUInt8):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kInt8):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kChar):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kUndefined): {
                value_size = 1 * value_count;
                break;
            }

            case std::to_underlying(DirectoryEntry::ValueTypeCode::kUInt16):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kInt16): {
                value_size = 2 * value_count;
                break;
            }

            case std::to_underlying(DirectoryEntry::ValueTypeCode::kUInt32):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kInt32):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kSingle): {
                value_size = 4 * value_count;
                break;
            }

            case std::to_underlying(DirectoryEntry::ValueTypeCode::kDouble): {
                value_size = 8 * value_count;
                break;
            }

            case std::to_underlying(DirectoryEntry::ValueTypeCode::kURational):
                [[fallthrough]];
            case std::to_underlying(DirectoryEntry::ValueTypeCode::kRational): {
                value_size = 16 * value_count;
                break;
            }

            default: {
                throw std::runtime_error("Unknown value type");
            }
        }

        // The field contains either VALUES or IFD OFFSET
        if (value_size <= value_capacity) {
            switch (tag) {
                // The field contains EXIF IFD OFFSET
                case 0x8769:
                    [[fallthrough]];
                // The field contains GPS IFD OFFSET
                case 0x8825: {
                    std::array<char, DirectoryEntry::FieldWidth::IfdOffset> new_ifd_offset_buf{};
                    ifs.read(new_ifd_offset_buf.data(), new_ifd_offset_buf.size());
                    auto new_ifd_offset = std::bit_cast<DirectoryEntry::FieldType::IfdOffset>(new_ifd_offset_buf);
                    if (std::endian::native != seg_.byte_order_) {
                        new_ifd_offset = std::byteswap(new_ifd_offset);
                    }

                    // Spawn a thread to parse the IFD
                    std::thread new_ifd_thread{&Parser::ParseIfd, this, static_cast<std::streampos>(new_ifd_offset)};
                    new_ifd_thread.join();
                    break;
                }

                // The field contains VALUES
                default: {
                    de_ptr->values_.resize(DirectoryEntry::FieldWidth::Values);
                    ifs.read(de_ptr->values_.data(), static_cast<std::streamsize>(DirectoryEntry::FieldWidth::Values));
                }
            }

            // The field contains VALUE OFFSET
        } else {
            // Parse and skip the VALUE OFFSET field of the DE
            std::array<char, DirectoryEntry::FieldWidth::ValueOffset> value_offset_buf{};
            ifs.read(value_offset_buf.data(), value_offset_buf.size());
            auto value_offset = std::bit_cast<DirectoryEntry::FieldType::ValueOffset>(value_offset_buf);
            if (std::endian::native != seg_.byte_order_) {
                value_offset = std::byteswap(value_offset);
            }

            // Anchor the current position for later return
            std::streampos curr_pos = ifs.tellg();

            // Go to the location where the actual values are stored
            ifs.seekg(seg_.ifh_base_ + static_cast<std::streamoff>(value_offset));

            // Read the actual values
            de_ptr->values_.resize(value_size);
            ifs.read(de_ptr->values_.data(), static_cast<std::streamsize>(value_size));

            // Return to the anchored position
            ifs.seekg(curr_pos);
        }

        de_ptr->tag_        = tag;
        de_ptr->value_type  = value_type;
        de_ptr->value_count = value_count;
        ifd_ptr->des_.push_back(std::move(de_ptr));
    }

    // Lock the `ifds_` mutex for thread safety
    std::lock_guard<std::mutex> ifds_lock(ifds_mutex_);
    seg_.ifds_.push_back(std::move(ifd_ptr));
}

}  // namespace lynx::exif
