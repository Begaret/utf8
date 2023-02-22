#pragma once
#include "codepoint.h"

namespace utf8
{
    class string
    {
        using iterator = codepoint*;

        public:
        /* Constructors */
        string() noexcept;
        string(codepoint cpt) noexcept;
        string(std::string str) noexcept;
        string(const char* str) noexcept;
        string(codepoint* cpts) noexcept;

        string(const string& other) noexcept;

        /* Destructor */
        ~string() noexcept;

        /* I/O */
        friend std::ostream& operator<<(std::ostream& os, const string& str);

        /* Access */
        iterator begin() noexcept { return &m_buffer[0]; }
        iterator end() noexcept { return &m_buffer[m_length]; }
        const iterator begin() const noexcept { return &m_buffer[0]; }
        const iterator end() const noexcept { return &m_buffer[m_length]; }

        codepoint& operator[](size_t index) noexcept { return m_buffer[index]; }
        codepoint& at(size_t index);

        codepoint operator[](size_t index) const noexcept { return m_buffer[index]; }
        codepoint at(size_t index) const;

        /* Comparison */
        bool operator==(const string& other) const;

        /* Concatenation */
        string& operator+=(const string& other);
        string operator+(const string& other) const;

        /* Members */
        inline codepoint* buffer() const noexcept { return m_buffer; }
        inline size_t length() const noexcept { return m_length; }

        private:
        codepoint* m_buffer;
        size_t m_length;
    };
}
