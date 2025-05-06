#include "jfif_segment.hpp"

#include <iostream>

namespace lynx {

JfifSegment::JfifSegment(uint16_t marker, uint16_t length) : ApplicationSegment{marker, length} {
    std::cout << "Creating JFIF Segment" << std::endl;
}

JfifSegment::~JfifSegment() {
    std::cout << "Deleting JFIF Segment" << std::endl;
}

void JfifSegment::Populate() {
    std::cout << "Populating JFIF Segment" << std::endl;
}

}  // namespace lynx
