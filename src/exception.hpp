#pragma once

#include <exception>
#include <string>

namespace bgt 
{
    namespace utf8
    {
        class invalid_utf8_exception : public std::exception
        {
            public:
            invalid_utf8_exception(const char* msg) noexcept;
            const char* what() const noexcept override;

            private:
            std::string m_codepoint;
        };

        class empty_utf8_exception : public std::exception
        {
            public:
            const char* what() const noexcept override;
        };

        class out_of_range_exception : public std::exception
        {
            public:
            const char* what() const noexcept override;
        };
    }
}