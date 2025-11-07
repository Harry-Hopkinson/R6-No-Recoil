#pragma once

#include <cstring>

namespace StringUtils
{

    /**
     * @brief Builds a file path with extension
     * @param dir Directory path
     * @param filename Filename without extension
     * @param ext Extension (including dot, e.g., ".bmp")
     * @return Newly allocated string with full path (caller must delete[])
     */
    inline char* BuildPath(const char* dir, const char* filename, const char* ext)
    {
        if (!dir || !filename || !ext) return nullptr;

        const size_t len1 = strlen(dir);
        const size_t len2 = strlen(filename);
        const size_t len3 = strlen(ext);
        const size_t totalLen = len1 + len2 + len3 + 1;

        char* result = new char[totalLen];
        strcpy_s(result, totalLen, dir);
        strcat_s(result, totalLen, filename);
        strcat_s(result, totalLen, ext);

        return result;
    }

    /**
     * @brief Creates a copy of a string
     * @param input String to copy
     * @return Newly allocated copy (caller must delete[])
     */
    inline char* CopyString(const char* input)
    {
        if (!input) return nullptr;

        const size_t len = strlen(input);
        char* result = new char[len + 1];
        strcpy_s(result, len + 1, input);
        return result;
    }

    /**
     * @brief Trims leading and trailing whitespace from a string in-place
     * @param str String to trim
     * @return Pointer to the trimmed string
     */
    inline char* TrimWhitespace(char* str)
    {
        if (!str) return nullptr;

        // Trim leading
        while (*str == ' ' || *str == '\t')
            ++str;

        if (*str == '\0') return str;

        // Trim trailing
        char* end = str + strlen(str) - 1;
        while (end > str && (*end == ' ' || *end == '\t'))
            *end-- = '\0';

        return str;
    }

    /**
     * @brief Checks if two strings are equal
     * @param a First string
     * @param b Second string
     * @return true if strings are equal, false otherwise
     */
    inline bool StringEquals(const char* a, const char* b)
    {
        if (!a || !b) return (a == b);
        return strcmp(a, b) == 0;
    }

    /**
     * @brief Checks if a string starts with a prefix
     * @param str String to check
     * @param prefix Prefix to look for
     * @return true if str starts with prefix
     */
    inline bool StartsWith(const char* str, const char* prefix)
    {
        if (!str || !prefix) return false;
        return strncmp(str, prefix, strlen(prefix)) == 0;
    }

    /**
     * @brief Parses a comma-separated weapon list into an array
     * @param weaponStr Comma-separated weapon string
     * @param weapons Output array to store weapon names (max maxWeapons)
     * @param maxWeapons Maximum number of weapons to parse
     * @return Number of weapons parsed
     */
    inline int ParseWeaponList(const char* weaponStr, const char* weapons[], int maxWeapons)
    {
        if (!weaponStr || !weapons || maxWeapons <= 0) return 0;

        int weaponCount = 0;
        const char* ptr = weaponStr;

        while (*ptr && weaponCount < maxWeapons)
        {
            // Skip leading spaces
            while (*ptr == ' ')
                ++ptr;

            const char* start = ptr;

            // Find end of token
            while (*ptr && *ptr != ',')
                ++ptr;

            int len = static_cast<int>(ptr - start);

            if (len > 0)
            {
                char* weaponName = new char[len + 1];
                strncpy_s(weaponName, len + 1, start, len);

                // Trim trailing spaces
                while (len > 0 && weaponName[len - 1] == ' ')
                    weaponName[--len] = '\0';

                if (len > 0)
                    weapons[weaponCount++] = weaponName;
                else
                    delete[] weaponName;
            }

            // Skip comma
            if (*ptr == ',') ++ptr;
        }

        return weaponCount;
    }

    /**
     * @brief Frees memory allocated for weapon names array
     * @param weapons Array of weapon name pointers
     * @param count Number of weapons in the array
     */
    inline void FreeWeaponList(const char* weapons[], int count)
    {
        if (!weapons) return;

        for (int i = 0; i < count; i++)
        {
            if (weapons[i]) delete[] weapons[i];
        }
    }

} // namespace StringUtils
