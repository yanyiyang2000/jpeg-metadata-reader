#include <iostream>
#include <memory> // smart pointer

#include "application_segment.hpp"
#include "jfif_segment.hpp"

int main() {
    lynx::JfifSegment jfif_seg{123, 456};

    lynx::ApplicationSegment& app_seg_ref{jfif_seg};
    
    app_seg_ref.Populate();

    std::cout << app_seg_ref.m_marker << std::endl;
    std::cout << app_seg_ref.m_length << std::endl;

    std::unique_ptr<lynx::ApplicationSegment> app_seg_ptr{new lynx::JfifSegment{123, 456}}; 
    
    std::cout << app_seg_ptr->m_marker << std::endl;
    std::cout << app_seg_ptr->m_length << std::endl;

    return 0;
}
