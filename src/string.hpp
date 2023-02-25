#pragma once

#include "codepoint.h"

namespace bgt 
{
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

            void append(const string& other) noexcept;
            void push(const codepoint& cpt) noexcept;
            codepoint pop() noexcept;

            /* Conversions (from string) */
            int to_int() const;
            unsigned int to_uint() const;
            long to_long() const;
            unsigned long to_ulong() const;
            long long to_llong() const;
            unsigned long long to_ullong() const;

            float to_float() const;     // TODO:
            double to_double() const;   // TODO:

            bool to_bool() const;

            /* Members */
            inline codepoint* buffer() const noexcept { return m_buffer; }
            inline size_t length() const noexcept { return m_length; }

            private:
            codepoint* m_buffer;
            size_t m_length;
        };

        /* Conversions (to string) */
        string to_string(int val) noexcept;
        string to_string(unsigned int val) noexcept;
        string to_string(long val) noexcept;
        string to_string(unsigned long val) noexcept;
        string to_string(long long val) noexcept;
        string to_string(unsigned long long val) noexcept;

        string to_string(float val) noexcept;
        string to_string(double val) noexcept;

        string to_string(bool val) noexcept;

        /* Conversions (from string) */
        inline int string_to_int(string str) { return str.to_int(); }
        inline unsigned int string_to_uint(string str) { return str.to_uint(); }
        inline long string_to_long(string str) { return str.to_long(); }
        inline unsigned long string_to_ulong(string str) { return str.to_ulong(); }
        inline long long string_to_llong(string str) { return str.to_llong(); }
        inline unsigned long long string_to_ullong(string str) { return str.to_ullong(); }

        inline float string_to_float(string str) { return str.to_float(); }
        inline double string_to_double(string str) { return str.to_double(); }

        inline bool string_to_bool(string str) { return str.to_bool(); }
    }
}