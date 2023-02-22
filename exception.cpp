#include "exception.h"

namespace utf8 {
    invalid_utf8_exception::invalid_utf8_exception(const char *cdpt) noexcept : m_codepoint(cdpt) {}

    const char *invalid_utf8_exception::what() const noexcept
    {
        // return ("invalid utf8 codepoint: '" + m_codepoint + "'").c_str();
        return "invalid utf8 codepoint";
    }

    const char *empty_utf8_exception::what() const noexcept 
    {
        return "utf8 codepoint is null";
    }

    const char *out_of_range_exception::what() const noexcept
    {
        return "index out of range";
    }
}
