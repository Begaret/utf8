#include "codepoint.hpp"

#include <iostream>

namespace bgt 
{
    namespace utf8 
    {
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

        bool codepoint::operator==(const codepoint &other) const noexcept
        {
            if (other.m_length != m_length)
                return false;
            
            for (int i = 0; i < m_length; i++)
                if (other.m_bytes[i] != m_bytes[i])
                    return false;
                
            return true;
        }

        std::ostream &operator<<(std::ostream &os, const codepoint &cpt)
        {
            if (cpt.m_bytes.empty())
                throw empty_utf8_exception();
            
            std::string str(cpt.m_bytes.begin(), cpt.m_bytes.end());
            str.push_back('\0');

            os << str;
            return os;
        }

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
}