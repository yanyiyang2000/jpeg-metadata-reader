#include "common.hpp"

#include <fstream>  // ifstream
#include <ios>      // streampos

namespace lynx::common {

Parser::Parser(std::ifstream& ifs, std::streampos seg_base) : ifs_{ifs}, seg_base_{seg_base} {
}

}  // namespace lynx::common
