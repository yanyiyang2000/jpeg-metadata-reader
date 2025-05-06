add_library(
    lynx
    STATIC
    application_segment.cpp
    jfif_segment.cpp
)

target_include_directories(
    lynx
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

