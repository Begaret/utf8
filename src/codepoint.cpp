#include "codepoint.h"

#include <iostream>

namespace utf8 {
    codepoint::codepoint() noexcept : m_length(0) {}

    codepoint::codepoint(char chr) noexcept : m_length(1)
    {
        m_bytes.push_back(chr);
    }

    codepoint::codepoint(const char *str) : m_length(strlen(str))
    {
        if (is_valid((const unsigned char*)str)) {
            while (*str) m_bytes.push_back(*str++);
        } else {
            throw invalid_utf8_exception(str);
        }
    }

    codepoint::codepoint(std::string str) : m_length(str.length())
    {
        if (is_valid(str)) {
            for (auto chr: str) {
                m_bytes.push_back(chr);
            }
        } else {
            throw invalid_utf8_exception(str.c_str());
        }
    }

    codepoint::codepoint(const codepoint &other) noexcept : m_bytes(other.m_bytes), m_length(other.m_length) {}

    codepoint &codepoint::operator=(const codepoint &other) noexcept
    {
        // avoid self-assignment
        if (&other != this) {
            m_bytes = other.m_bytes;
            m_length = other.m_length;
        }

        return *this;
    }

    codepoint::~codepoint() noexcept {}

    std::ostream &operator<<(std::ostream &os, const codepoint &cpt)
    {
        if (cpt.m_bytes.empty())
            throw empty_utf8_exception();

        std::string str(cpt.m_bytes.begin(), cpt.m_bytes.end());
        str.push_back('\0');

        os << str;
        return os;
    }
}
