#include "common.hpp"

#include <iosfwd>  // streampos
#include <string>  // string

namespace lynx::common {

Parser::Parser(std::string file_name, const std::streampos seg_base) : file_name_{file_name}, seg_base_{seg_base} {
}

}  // namespace lynx::common
