#ifndef LYNX_JPEG_PARSER_HPP
#define LYNX_JPEG_PARSER_HPP

#include <string>

namespace lynx::jpeg {

/**
 * @brief JPEG metadata parser
 */
class Parser {
private:
    const std::string file_name_;

public:
    /**
     * @brief Constructor
     *
     * @note This is an explicitly-defaulted constructor.
     */
    Parser(std::string file_name);

    /**
     * @brief Destructor
     *
     * @note This is an explicitly-defaulted destructor.
     */
    ~Parser() = default;

    /**
     * @brief Parses the JPEG metadata.
     */
    void Parse();
};

}  // namespace lynx::jpeg

#endif  // LYNX_JPEG_PARSER_HPP
