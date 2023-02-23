#pragma once

/*
 *  A class acting as a wrapper over UTF-8 encoded characters
 */

#include <string>
#include <vector>

#include "exception.h"
#include "validate.h"

namespace utf8 {
    class codepoint
    {
        public:
        /*  Constructors */
        codepoint() noexcept;
        codepoint(char chr) noexcept;
        codepoint(const char* str);
        codepoint(std::string str);

        codepoint(const codepoint& other) noexcept;

        /* Assignment */
        codepoint& operator=(const codepoint& other) noexcept;

        /*  Destructor */
        ~codepoint() noexcept;

        /* access */
        inline std::vector<unsigned char> bytes() const noexcept { return m_bytes; }

        inline size_t length() const noexcept { return m_length; }

        /* printing */
        friend std::ostream& operator<<(std::ostream& os, const codepoint& cpt);

        inline std::string str() const noexcept {
            return std::string(m_bytes.begin(), m_bytes.end()); 
        }

        /* miscellaneous */
        inline operator bool() const noexcept { return m_length != 0; }

        private:
        std::vector<unsigned char> m_bytes;
        size_t m_length;
    };


    /// @brief returns the a vector of all codepoint in a given string, or an empty one if search failed
    /// @param str the string to search
    /// @param index the starting index
    /// @param include_ascii whether or not to include ascii characters
    /// @return returns a vector containing all codepoints in string
    std::vector<codepoint> get_codepoints(std::string str, size_t index = 0, bool include_ascii = false) noexcept;

    /// @brief returns the first codepoint in a given string, or an empty one if search failed
    /// @param str the string to search
    /// @param index the starting index
    /// @param include_ascii whether or not to include ascii characters in search
    /// @return returns the first codepoint found
    codepoint get_codepoint(std::string str, size_t index = 0, bool include_ascii = false) noexcept;
    
    /// @brief searches a string and returns the index of the first codepoint or -1 if search failed
    /// @param str the string to search
    /// @param index the starting index
    /// @param include_ascii whether or not to include ascii characters in search
    /// @return returns the index in chars NOT codepoints
    size_t find_codepoint(std::string str, size_t index = 0, bool include_ascii = false) noexcept;
    
    /// @brief searches a string for a specified codepoint
    /// @param str the string to search
    /// @param target the codepoint to look for
    /// @param index the starting index
    /// @return returns the index of the given target or -1 if search failed. NOTE index is in chars NOT codepoints
    size_t find_codepoint(std::string str, codepoint target, size_t index = 0) noexcept;
}
