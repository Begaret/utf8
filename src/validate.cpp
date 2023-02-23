#include "validate.h"

namespace utf8 {
    static bool rest_is_valid(const unsigned char* bytes)
    {
        int i = 1;
        while (bytes[i]) {
            if (bytes[i] < 0x80 || bytes[i] > 0xBF)
                return false;
            i++;
        }

        return true;
    }

    bool is_valid(const unsigned char* bytes)
    {
        int required_bytes = 0;

        if (!bytes) return false;
        
        // count bytes. If there are more than
        // four bytes, the string is invalid
        int i = 0;
        while (bytes[i]) {
            if (i > UTF8_MAX)
                return false;
            i++;
        }

        // ASCII
        if (bytes[0] <= 0x7F) {
            required_bytes = 1;
        }

        else if (bytes[0] >= 0xC2 && bytes[0] <= 0xDF) {
            required_bytes = 2;
        }

        else if (bytes[0] >= 0xE0 && bytes[0] <= 0xEF) {
            required_bytes = 3;
        }

        else if (bytes[0] >= 0xF0 && bytes[0] <= 0xFF) {
            required_bytes = 4;
        }

        else return false;

        if (required_bytes > 1) {
            return i == required_bytes && rest_is_valid(bytes);
        } else {
            return i == required_bytes;
        }
    }
}
