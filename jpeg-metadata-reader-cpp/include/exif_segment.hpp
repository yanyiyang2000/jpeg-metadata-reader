#ifndef LYNX_EXIF_SEGMENT_HPP
#define LYNX_EXIF_SEGMENT_HPP

#include "application_segment.hpp"

namespace lynx {

class ExifSegment : public ApplicationSegment {
public:
    ExifSegment();
    
    ~ExifSegment();
};

} // namespace lynx

#endif  // LYNX_EXIF_SEGMENT_HPP
