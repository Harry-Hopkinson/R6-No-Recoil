#pragma once

namespace StringUtils
{
    /**
     * @brief Removes all spaces from the input string
     * @param input The input string to process
     * @return A newly allocated string with spaces removed (caller must delete[])
     */
    char* RemoveSpaces(const char* input);

    /**
     * @brief Concatenates directory path, filename, and .bmp extension
     * @param dir The directory path
     * @param filename The filename without extension
     * @return A newly allocated string with the full path (caller must delete[])
     */
    char* BuildPath(const char* dir, const char* filename);

    /**
     * @brief Creates a copy of the input string
     * @param input The string to copy
     * @return A newly allocated copy of the string (caller must delete[])
     */
    char* CreateStringCopy(const char* input);

    /**
     * @brief Parses a comma-separated weapon list into an array
     * @param weaponStr The comma-separated weapon string
     * @param weapons Output array to store weapon names (max 3)
     * @param maxWeapons Maximum number of weapons to parse
     * @return Number of weapons parsed
     */
    int ParseWeaponList(const char* weaponStr, const char* weapons[], int maxWeapons);

    /**
     * @brief Frees memory allocated for weapon names array
     * @param weapons Array of weapon name pointers
     * @param count Number of weapons in the array
     */
    void FreeWeaponList(const char* weapons[], int count);
} // namespace StringUtils
