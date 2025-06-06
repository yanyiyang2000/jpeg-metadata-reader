add_library(
    lynx
    STATIC
    common.cpp
    jfif_parser.cpp
    exif_parser.cpp
    jpeg_parser.cpp
)

target_include_directories(
    lynx
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include/private"
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include/public"
)

