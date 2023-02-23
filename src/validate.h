#pragma once

#define UTF8_MAX 4

#include <string>

namespace utf8
{
    inline bool is_valid(const unsigned char chr)
    {
        return chr >= 0 && chr <= 0x7F;
    }

    bool is_valid(const unsigned char* bytes);

    inline bool is_valid(const std::string& str)
    {
        return is_valid((const unsigned char*)str.c_str());
    }
}
