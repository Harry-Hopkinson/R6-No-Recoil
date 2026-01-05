#pragma once

#include <cstring>

static inline int is_space(int c)
{
    return (
        c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
        || c == '\r');
}

namespace StringUtils
{

    /**
     * @brief Builds a file path with extension
     * @param dir Directory path
     * @param filename Filename without extension
     * @param ext Extension (including dot, e.g., ".bmp")
     * @return Newly allocated string with full path
     */
    inline char* BuildPath(
        const char* dir, const char* filename, const char* ext)
    {
        if (!dir || !filename || !ext)
            return nullptr;

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
     * @return Newly allocated copy
     */
    inline char* CopyString(const char* input)
    {
        if (!input)
            return nullptr;

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
        if (!str)
            return nullptr;

        // Trim leading
        while (*str == ' ' || *str == '\t')
            ++str;

        if (*str == '\0')
            return str;

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
        if (!a || !b)
            return (a == b);
        return strcmp(a, b) == 0;
    }

    /**
     * @brief Parses a comma-separated weapon list into an array
     * @param weaponStr Comma-separated weapon string
     * @param weapons Output array to store weapon names (max 3)
     * @return Number of weapons parsed
     */
    inline int ParseWeaponList(const char* weaponStr, const char* weapons[])
    {
        if (!weaponStr || !weapons)
            return 0;

        int weaponCount = 0;
        const char* ptr = weaponStr;

        while (*ptr && weaponCount < 3)
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
            if (*ptr == ',')
                ++ptr;
        }

        return weaponCount;
    }

    /**
     * @brief Retrieves the weapon name at a specific index
     * @param weapons Comma-separated weapon string
     * @param index Index of the weapon to retrieve
     * @param out Output buffer to store the weapon name
     * @param out_size Size of the output buffer
     */
    inline void GetWeaponAtIndex(
        const char* weapons, int index, char* out, size_t out_size)
    {
        if (!weapons || !out || out_size == 0)
            return;

        int current = 0;
        const char* start = weapons;

        while (*start)
        {
            while (*start && is_space((unsigned char)*start))
                ++start;

            const char* end = start;
            while (*end && *end != ',')
                ++end;

            if (current == index)
            {
                const char* trim_end = end;
                while (trim_end > start
                       && is_space((unsigned char)*(trim_end - 1)))
                    --trim_end;

                size_t len = trim_end - start;
                if (len >= out_size)
                    len = out_size - 1;

                memcpy(out, start, len);
                out[len] = '\0';
                return;
            }

            if (*end == ',')
                ++end;

            start = end;
            ++current;
        }

        out[0] = '\0';
    }

    /**
     * @brief Frees memory allocated for weapon names array
     * @param weapons Array of weapon name pointers
     * @param count Number of weapons in the array
     */
    inline void FreeWeaponList(const char* weapons[], int count)
    {
        if (!weapons)
            return;

        for (int i = 0; i < count; i++)
        {
            if (weapons[i])
                delete[] weapons[i];
        }
    }

} // namespace StringUtils
