#include "string.h"

#include <cmath>

namespace bgt 
{
    namespace utf8 
    {
        string::string() noexcept : m_length(0) 
        {
            m_buffer = new codepoint[1];
            *m_buffer = '\0';
        }

        string::string(codepoint cpt) noexcept : m_length(1)
        {
            m_buffer = new codepoint[2];
            m_buffer[0] = cpt;
            m_buffer[1] = '\0';
        }

        string::string(std::string str) noexcept
        {
            if (is_valid(str)) {
                m_buffer = new codepoint[2];
                m_buffer[0] = str;
                m_buffer[1] = '\0';
                m_length = 1;
            } else {
                int len = str.length();
                m_buffer = new codepoint[len + 1];

                int i = 0;
                for (auto cpt: get_codepoints(str, 0, true)) {
                    m_buffer[i++] = cpt;
                } m_length = i;

                m_buffer[len] = '\0';
            }
        }

        string::string(const char *str) noexcept
        {
            if (!str) {
                m_buffer = new codepoint[0];
                m_length = 0;
            }

            else {
                if (is_valid(str)) {
                    m_buffer = new codepoint[2];
                    m_buffer[0] = str;
                    m_buffer[1] = '\0';
                    m_length = 1;
                } else {
                    int len = strlen(str);
                    m_buffer = new codepoint[len + 2];

                    int i = 0;
                    for (auto cpt: get_codepoints(str, 0, true)) {
                        m_buffer[i++] = cpt;
                    } m_length = i;

                    m_buffer[len] = '\0';
                }
            }
        }

        string::string(codepoint *cpts) noexcept
        {
            if (!cpts) {
                m_buffer = new codepoint[0];
                m_length = 0;
            }

            else {
                int i = 0;
                while (cpts[i] != '\0') i++;
                m_length = i;
                m_buffer = new codepoint[i + 1];

                std::copy(cpts, cpts + m_length, m_buffer);
                m_buffer[m_length] = '\0';
            }
        }

        string::string(const string &other) noexcept
        {
            codepoint* buffer = new codepoint[other.m_length + 1];
            std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer);
            buffer[other.m_length] = '\0';

            m_buffer = buffer;
            m_length = other.m_length;
        }

        string::~string() noexcept
        {
            delete [] m_buffer;
            m_buffer = nullptr;
        }

        codepoint &string::at(size_t index)
        {
            if (index > 0 && index < m_length) {
                return m_buffer[index];
            } else {
                throw out_of_range_exception();
            }
        }

        codepoint string::at(size_t index) const
        {
            if (index > 0 && index < m_length) {
                return m_buffer[index];
            } else {
                throw out_of_range_exception();
            }
        }

        bool string::operator==(const string &other) const
        {
            if (other.m_length != m_length)
                return false;

            for (int i = 0; i < other.m_length; i++)
                if (other.m_buffer[i] != m_buffer[i])
                    return false;
            return true;
        }

        string &string::operator+=(const string &other)
        {
            if (this != &other)
            {
                size_t new_length = m_length + other.m_length;
                codepoint* buffer = new codepoint[new_length + 1];

                std::copy(m_buffer, m_buffer + m_length, buffer);
                std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer + m_length);
                buffer[new_length] = '\0';

                std::swap(m_buffer, buffer);
                m_length = new_length;

                delete [] buffer;
            }

            return *this;
        }

        string string::operator+(const string &other) const
        {
            const size_t new_length = m_length + other.m_length;
            codepoint* buffer = new codepoint[new_length + 1];

            std::copy(m_buffer, m_buffer + m_length, buffer);
            std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer + m_length);
            buffer[new_length] = '\0';

            return string(buffer);
        }

        void string::append(const string &other) noexcept
        {
            size_t new_length = m_length + other.m_length;
            codepoint* buffer = new codepoint[new_length + 1];

            std::copy(m_buffer, m_buffer + m_length, buffer);
            std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer + m_length);
            buffer[new_length] = '\0';

            std::swap(m_buffer, buffer);
            m_length = new_length;

            delete [] buffer;
        }

        void string::push(const codepoint &cpt) noexcept
        {
            size_t new_length = m_length + 1;
            codepoint* buffer = new codepoint[new_length + 1];

            std::copy(m_buffer, m_buffer + m_length, buffer);
            buffer[m_length] = cpt;
            buffer[new_length] = '\0';

            std::swap(m_buffer, buffer);
            m_length = new_length;

            delete [] buffer;
        }

        codepoint string::pop() noexcept
        {
            // get last element of buffer
            auto cpt = m_buffer[m_length - 1];

            size_t new_length = m_length - 1;
            codepoint* buffer = new codepoint[new_length + 1];

            std::copy(m_buffer, m_buffer + (new_length - 1), buffer);
            buffer[new_length] = '\0';

            std::swap(m_buffer, buffer);
            m_length = new_length;

            delete [] buffer;
            return cpt;
        }

        std::ostream &operator<<(std::ostream &os, const string &str)
        {
            int i = 0;
            while (i < str.m_length)
                os << str.m_buffer[i++];

            return os;
        }

        int string::to_int() const
        {
            int ret = 0;
            bool neg = m_buffer[0] == '-';
            for (int i = m_length - 1; i > (neg ? 1 : 0); i--) {
                if (m_buffer[i] == '.') break;
                if (!isnumber(m_buffer[i].bytes()[0])) throw;
                ret = ret + ((int(m_buffer[i].bytes()[0]) - '0') * (pow(10, (m_length - i) - 1)));
            }

            return neg ? ret - (ret * 2) : ret;
        }

        unsigned int string::to_uint() const
        {
            unsigned int ret = 0;
            for (int i = m_length - 1; i > 0; i--) {
                if (m_buffer[i] == '.') break;
                if (!isnumber(m_buffer[i].bytes()[0])) throw;
                ret = ret + ((int(m_buffer[i].bytes()[0]) - '0') * (pow(10, (m_length - i) - 1)));
            }

            return ret;
        }

        long string::to_long() const
        {
            long ret = 0;
            bool neg = m_buffer[0] == '-';
            for (int i = m_length - 1; i > (neg ? 1 : 0); i--) {
                if (m_buffer[i] == '.') break;
                if (!isnumber(m_buffer[i].bytes()[0])) throw;
                ret = ret + ((int(m_buffer[i].bytes()[0]) - '0') * (pow(10, (m_length - i) - 1)));
            }

            return neg ? ret - (ret * 2) : ret;
        }

        unsigned long string::to_ulong() const
        {
            unsigned long ret = 0;
            for (int i = m_length - 1; i > 0; i--) {
                if (m_buffer[i] == '.') break;
                if (!isnumber(m_buffer[i].bytes()[0])) throw;
                ret = ret + ((int(m_buffer[i].bytes()[0]) - '0') * (pow(10, (m_length - i) - 1)));
            }

            return ret;
        }

        long long string::to_llong() const
        {
            long long ret = 0;
            bool neg = m_buffer[0] == '-';
            for (int i = m_length - 1; i > (neg ? 1 : 0); i--) {
                if (m_buffer[i] == '.') break;
                if (!isnumber(m_buffer[i].bytes()[0])) throw;
                ret = ret + ((int(m_buffer[i].bytes()[0]) - '0') * (pow(10, (m_length - i) - 1)));
            }

            return neg ? ret - (ret * 2) : ret;
        }

        unsigned long long string::to_ullong() const
        {
            unsigned long long ret = 0;
            for (int i = m_length - 1; i > 0; i--) {
                if (m_buffer[i] == '.') break;
                if (!isnumber(m_buffer[i].bytes()[0])) throw;
                ret = ret + ((int(m_buffer[i].bytes()[0]) - '0') * (pow(10, (m_length - i) - 1)));
            }

            return ret;
        }

        float string::to_float() const
        {
            if (*this == "inf")
                return INFINITY;
            
            if (*this == "NaN")
                return NAN;

            float ret = 0.0f;
            bool neg = m_buffer[0] == '-';

            return neg ? ret - (ret * 2.0f) : ret;
        }

        bool string::to_bool() const
        {
            if (*this == "true")
                return true;
            else if (*this == "false")
                return false;
            throw;
        }

        string to_string(int val) noexcept
        {
            string ustr;

            int temp = abs(val);

            while (temp > 0) {
                int digit = temp % 10;
                temp /= 10;

                ustr.push(digit + '0');
            }

            string ustr2;
            if (val < 0) ustr2.push('-');
            for (int i = ustr.length(); i >= 0; i--) {
                ustr2.push(ustr[i]);
            }
            
            return ustr2;
        }

        string to_string(unsigned int val) noexcept
        {
            string ustr;

            unsigned int temp = val;

            while (temp > 0) {
                int digit = temp % 10;
                temp /= 10;

                ustr.push(digit + '0');
            }

            string ustr2;
            for (int i = ustr.length(); i >= 0; i--) {
                ustr2.push(ustr[i]);
            }
            
            return ustr2;
        }

        string to_string(long val) noexcept
        {
            string ustr;

            long temp = abs(val);

            while (temp > 0) {
                int digit = temp % 10;
                temp /= 10;

                ustr.push(digit + '0');
            }

            string ustr2;
            if (val < 0) ustr2.push('-');
            for (int i = ustr.length(); i >= 0; i--) {
                ustr2.push(ustr[i]);
            }
            
            return ustr2;
        }

        string to_string(unsigned long val) noexcept
        {
            string ustr;

            unsigned long temp = val;

            while (temp > 0) {
                int digit = temp % 10;
                temp /= 10;

                ustr.push(digit + '0');
            }

            string ustr2;
            if (val < 0) ustr2.push('-');
            for (int i = ustr.length(); i >= 0; i--) {
                ustr2.push(ustr[i]);
            }
            
            return ustr2;
        }

        string to_string(long long val) noexcept
        {
            string ustr;

            long long temp = abs(val);

            while (temp > 0) {
                int digit = temp % 10;
                temp /= 10;

                ustr.push(digit + '0');
            }

            string ustr2;
            if (val < 0) ustr2.push('-');
            for (int i = ustr.length(); i >= 0; i--) {
                ustr2.push(ustr[i]);
            }
            
            return ustr2;
        }

        string to_string(unsigned long long val) noexcept
        {
            string ustr;

            unsigned long long temp = val;

            while (temp > 0) {
                int digit = temp % 10;
                temp /= 10;

                ustr.push(digit + '0');
            }

            string ustr2;
            if (val < 0) ustr2.push('-');
            for (int i = ustr.length(); i >= 0; i--) {
                ustr2.push(ustr[i]);
            }
            
            return ustr2;
        }

        string to_string(float val) noexcept
        {
            if (std::isinf(val))
                return "inf";
                
            if (!std::isfinite(val))
                return "NaN";

            int i = int(val);
            float mantissa = val - i;

            string s = to_string(i);
            s.push('.');

            int digit = val;
            while (digit != 0) {
                mantissa *= 10.0f;
                digit = int(mantissa) % 10;
                s.push(digit + '0');
            }

            return s;
        }

        string to_string(double val) noexcept
        {
            if (std::isinf(val))
                return "inf";

            if (!std::isfinite(val))
                return "NaN";

            int i = int(val);
            double mantissa = val - i;

            string s = to_string(i);
            s.push('.');

            int digit = val;
            while (digit != 0) {
                mantissa *= 10.0f;
                digit = int(mantissa) % 10;
                s.push(digit + '0');
            }

            return s;
        }
        
        string to_string(bool val) noexcept
        {
            return val ? "true" : "false";
        }
    }
}