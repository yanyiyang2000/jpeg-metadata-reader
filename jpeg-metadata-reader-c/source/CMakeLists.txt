add_library(
    jpeg-reader
    STATIC
    jpeg.c
    jfif.c
    exif.c
)

target_include_directories(
    jpeg-reader
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include/public"
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include/private"
)
