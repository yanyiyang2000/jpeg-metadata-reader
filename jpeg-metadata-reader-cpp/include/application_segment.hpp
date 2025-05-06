#ifndef LYNX_APPLICATION_SEGMENT_HPP
#define LYNX_APPLICATION_SEGMENT_HPP

#include "stdint.h"

namespace lynx {

class ApplicationSegment {
public:
    uint16_t m_marker;
    uint16_t m_length;

    ApplicationSegment(uint16_t marker, uint16_t length);

    virtual ~ApplicationSegment() = default;

    virtual void Populate() = 0;  // pure virtual function (implementation depends on derived classes)
};

}  // namespace lynx

#endif  // LYNX_APPLICATION_SEGMENT_HPP
