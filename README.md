# Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [JPEG File Format](#jpeg-file-format)
    - [APP0](#app0)
    - [APP1](#app1)
    - [IFH](#ifh)
    - [IFD](#ifd)
    - [DE](#de)
- [Workflow](#workflow)
- [Constructing Exif Segment](#constructing-exif-segment)
- [Reference](#reference)


# Overview
- This library parses the Application Marker Segments of JPEG files to extract metadata.
- The C version is ready for use. (only little-endian machines are supported for now)
- The C++ version is currently under development.


# JPEG File Format
Metadata of a JPEG file is stored in multiple *Application Marker Segments* (**APP**).
| Segment    | Marker        | Length   |
| ---------- | ------------- | -------- |
| SOI        | `FF D8`[^1.1] | 0[^1.2]  |
| APP0       | `FF E0`       | variable |
| APP1       | `FF E1`       | variable |
| ...        | ...           | ...      |
| APPn       | `FF En`       | variable |
| DQT        | `FF DB`       | variable |
| DHT1       | `FF C4`       | variable |
| DHT2       | `FF C4`       | variable |
| ...        | ...           | ...      |
| DHTn       | `FF C4`       | variable |
| DRI        | `FF DD`       | 4        |
| SOS        | `FF DA`       | variable |
| Image Data | -             | variable |
| EOI        | `FF D9`       | variable |

> [!NOTE]
> The JPEG file format is specified in **ISO 10918-1:1994**, p.33.

[^1.1]: Unless otherwise stated, all monospace texts are in hexadecimal
[^1.2]: Unless otherwise stated, all lengths are in bytes

We only care about the information stored in **APP0** (**JFIF Marker Segment**) and **APP1** (**EXIF Marker Segment**).

## APP0
APP0 (JFIF Marker Segment) is composed of the following fields:
| Description | Length   | Value                  |
| ----------- | -------- | ---------------------- |
| Marker      | 2        | `FF E0`                |
| Length      | 2        | [^2.1]                 |
| Identifier  | 5        | `4A 46 49 46 00`[^2.2] |
| Version     | 2        |                        |
| Unit        | 1        |                        |
| Xdensity    | 2        |                        |
| Ydensity    | 2        |                        |
| Xthumbnail  | 1        |                        |
| Ythumbnail  | 1        |                        |

> [!NOTE]
> The format of JFIF Marker Segment is specified in **JFIF Version 1.02**, p.5.

[^2.1]: All values of **APP0** are in big-endian
[^2.2]: ASCII string "JFIF" terminated by a null byte

## APP1
APP1 (Exif Marker Segment) is composed of a *Image File Header* (**IFH**) and multiple *Image File Directories* (**IFD**s).
| Description           | Length   | Value                     |
| --------------------- | -------- | ------------------------- |
| Marker                | 2        | `FF E1`                   |
| Length                | 2        | [^3.1]                    |
| Identifier            | 6        | `45 78 69 66 00 00`[^3.2] |
| IFH                   | 8        | See [IFH](#ifh)           |
| 0th IFD               | variable | See [IFD](#ifd)           |
| 1st IFD               | variable |                           |
| ...                   | ...      |                           |
| nth IFD               | variable |                           |

> [!NOTE]
> The format of Exif Marker Segment is specified in **Exif Version 3.0**, p.31.

[^3.1]: Length is always in big-endian
[^3.2]: ASCII string "Exif" terminated by two null bytes

## IFH
An IFH is commposed of the following fields:
| Description             | Length   | Value   |
| ----------------------- | -------- | ------- |
| Byte Order[^4.1]        | 2        | [^4.2]  |
| Magic Number            | 2        | [^4.3]  |
| Offset of 0th IFD[^4.4] | 4        |         |

> [!NOTE]
> The format of IFH is specified in **TIFF Revision 6.0**, p.13.

[^4.1]: Byte Order dictates the endianess of the following values
[^4.2]: `49 49` for little-endian and `4D 4D` for big-endian
[^4.3]: `2A 00` for little-endian and `00 2A` for big-endian
[^4.4]: Offset is from the first byte of IFH

## IFD
An IFD is composed of multiple *Directory Entries* (**DE**s).
| Description              | Length   | Value   |
| ------------------------ | -------- | ------- |
| DE Count                 | 2        |         |
| DE 1                     | 12       |         |
| ...                      | ...      | ...     |
| DE n                     | 12       |         |
| Offset of Next IFD[^5.1] | 4        | [^5.2]  |
| IFD Value                | variable |         |

> [!NOTE]
> The format of IFH is specified in **TIFF Revision 6.0**, pp.14-16.

[^5.1]: Offset is from the first byte of IFH
[^5.2]: `00 00 00 00` for the last IFD

## DE
A DE is composed of the following fields:
| Description        | Length   | Value  |
| ------------------ | -------- | ------ |
| Tag                | 2        |        |
| Type               | 2        |        |
| Value Count        | 4        |        |
| Value/Offset[^6.1] | 4        |        |

> [!NOTE]
> The format of DE is specified in **TIFF Revision 6.0**, pp.14-15.

[^6.1]: If 4 bytes are not enough to hold all the values, this value indicates the offset of the actual values from the first byte of IFH


# Parsing JPEG file
<img src="/assets/flowchart.png" width=50% height=50%>


# Parsing Exif Segment
<img src="/assets/flowchart_exif.png" width=50% height=50%>


# Reference
- ISO/IEC 10918-1 (JPEG)
- [JFIF Verion 1.02](/assets/JFIF_Version_1.02.pdf)
- [Exif Version 3.0](/assets/Exif_Version_3.0.pdf)
- [TIFF Revision 6.0](/assets/TIFF_Revision_6.0.pdf)
- [TIFF Tag Reference](https://www.awaresystems.be/imaging/tiff/tifftags.html)
