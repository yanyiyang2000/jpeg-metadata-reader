add_executable(
    demo 
    demo.c
)

target_link_libraries(
    demo
    PRIVATE
    jpeg-reader
)

target_include_directories(
    demo
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include/public"
)

install(
    TARGETS
    demo
    DESTINATION
    ${PROJECT_SOURCE_DIR}/example
)
