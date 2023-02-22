#include "utf8.h"

#include <cstring>

namespace utf8 {
    std::vector<codepoint> get_codepoints(std::string str, size_t index, bool include_ascii) noexcept
    {
        std::vector<codepoint> vec8;

        if (str.empty()) 
            return vec8;

        while (str[index] != '\0') {
            vec8.push_back(get_codepoint(str, index, include_ascii));
            index += vec8.back().length();
        }

        return vec8;
    }

    codepoint get_codepoint(std::string str, size_t index, bool include_ascii) noexcept
    {
        if (str.empty()) 
            return codepoint();

        std::string sub;

        while (index < str.length()) {
            if (is_valid(str[index]) && include_ascii)
                return codepoint(str[index]);

            sub = str.substr(index, 4);
            if (is_valid(sub)) {
                return codepoint(sub);
            }

            sub = str.substr(index, 3);
            if (is_valid(sub)) {
                return codepoint(sub);
            }

            sub = str.substr(index, 2);
            if (is_valid(sub)) {
                return codepoint(sub);
            }

            index++;
        }

        return codepoint();
    }

    size_t find_codepoint(std::string str, size_t index, bool include_ascii) noexcept
    {
        if (str.empty()) 
            return -1;

        std::string sub;

        while (index < str.length()) {
            if (is_valid(str[index]) && include_ascii)
                return index;

            sub = str.substr(index, 4);
            if (is_valid(sub)) {
                return index;
            }

            sub = str.substr(index, 3);
            if (is_valid(sub)) {
                return index;
            }

            sub = str.substr(index, 2);
            if (is_valid(sub)) {
                return index;
            }

            index++;
        }

        return -1;
    }

    size_t find_codepoint(std::string str, codepoint target, size_t index) noexcept
    {
        if (str.empty()) 
            return -1;

        std::string sub;

        while (index < str.length()) {
            if (is_valid(str[index]) && str[index] == target.str()[0])
                return index;

            sub = str.substr(index, 4);
            if (is_valid(sub) && sub == target.str()) {
                return index;
            }

            sub = str.substr(index, 3);
            if (is_valid(sub) && sub == target.str()) {
                return index;
            }

            sub = str.substr(index, 2);
            if (is_valid(sub) && sub == target.str()) {
                return index;
            }

            index++;
        }

        return -1;
    }
}
