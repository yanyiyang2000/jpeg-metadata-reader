/**
 * @file   jpeg.h
 *
 * @author Yiyang Yan
 *
 * @date   2024/07/20
 *
 * @brief  Functions to parse EXIF Segment.
 */

#ifndef EXIF_H
#define EXIF_H

#include <stdint.h>

/**
 * @brief Image File Directory byte orders
 *
 * Reference: TIFF Revision 6.0, p.13
 */
#define BYTE_ORDER_II 0x4949  // Little-endian
#define BYTE_ORDER_MM 0x4D4D  // Big-endian

/**
 * @brief Directory Entry field types
 *
 * Reference: TIFF Revision 6.0, pp.15-16
 */
#define BYTE      1   // 8-bit unsigned integer
#define ASCII     2   // 8-bit byte that contains a 7-bit ASCII code; the last byte must be NULL
#define SHORT     3   // 16-bit (2-byte) unsigned integer
#define LONG      4   // 32-bit (4-byte) unsigned integer
#define RATIONAL  5   // Two LONG's: the first represents the numerator of a fraction, the second the denominator
#define SBYTE     6   // 8-bit signed (two's-complement) integer
#define UNDEFINED 7   // 8-bit byte that may contain anything, depending on the definition of the field
#define SSHORT    8   // 16-bit (2-byte) signed (two's-complement) integer
#define SLONG     9   // 32-bit (4-byte) signed (two's-complement) integer
#define SRATIONAL 10  // Two SLONG's: the first represents the numerator of a fraction, the second the denominator
#define FLOAT     11  // Single precision (4-byte) IEEE format
#define DOUBLE    12  // Double precision (8-byte) IEEE format

/**
 * @brief EXIF Segment representation
 */
struct EXIF_Segment {
    uint8_t                     *IFH_Base;  // The pointer to the first byte of IFH
    struct Image_File_Directory *Next_IFD;  // The pointer to the next IFD
    struct Image_File_Directory *EXIF_IFD;  // The pointer to the EXIF IFD
    struct Image_File_Directory *GPS_IFD;   // The pointer to the GPS IFD
};

/**
 * @brief Image File Directory representation
 */
struct Image_File_Directory {
    uint16_t                     DE_Count;  // The number of DEs
    struct Image_File_Directory *Next_IFD;  // The pointer to the next IFD
    struct Directory_Entry      *DEs;       // The pointer to the first DE
};

/**
 * @brief Directory Entry representation
 */
struct Directory_Entry {
    uint16_t Tag;          // The tag of the DE
    uint16_t Value_Type;   // The type of values
    uint16_t Value_Count;  // The number of values
    uint8_t *Value;        // The pointer to the first value of the DE
};

/**
 * @brief Construct an EXIF Segment struct by parsing the given byte array.
 *
 * @param seg The pointer to the EXIF Segment struct
 * @param ptr The pointer to the pointer to the byte array
 *
 * @note Parameter `ptr` will be advanced by the length of the EXIF Segment.
 */
void exif_construct(struct EXIF_Segment *seg, uint8_t **ptr);

/**
 * @brief Parse the given EXIF Segment struct.
 *
 * @param seg The pointer to the EXIF Segment struct
 */
void exif_parse(struct EXIF_Segment *seg);

/**
 * @brief Free the memory dynamically allocated to the given EXIF Segment struct.
 *
 * @param seg The pointer to the EXIF Segment struct
 */
void exif_free(struct EXIF_Segment *seg);

/**
 * @brief Construct the given Image File Directory struct.
 *
 * @param seg      The pointer to the EXIF Segment struct
 * @param idx      The index of the Image File Directory struct to be parsed (0 = TIFF IFD, 1 = EXIF IFD, 2 = GPS IFD)
 * @param ifd_ofst The offset of the Image File Directory from the first byte of Image File Header
 */
void ifd_construct(struct EXIF_Segment *seg, uint8_t idx, uint32_t ifd_ofst);

/**
 * @brief Parse the Image File Directory struct specified by the index.
 *
 * @param seg The pointer to the EXIF Segment struct
 * @param idx The index of the Image File Directory struct to be parsed (0 = TIFF IFD, 1 = EXIF IFD, 2 = GPS IFD)
 */
void ifd_parse(struct EXIF_Segment *seg, uint8_t idx);

#endif /* EXIF_H */
