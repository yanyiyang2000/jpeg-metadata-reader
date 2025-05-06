add_executable(
    demo 
    demo.cpp
)

target_link_libraries(
    demo
    PRIVATE
    lynx
)

target_include_directories(
    demo
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

install(
    TARGETS
    demo
    DESTINATION
    ${PROJECT_SOURCE_DIR}/example
)
