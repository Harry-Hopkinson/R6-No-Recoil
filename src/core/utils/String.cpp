#include "String.hpp"

#include <cstring>
#include <cstdlib>

namespace StringUtils
{

char* RemoveSpaces(const char* input)
{
    if (!input) return nullptr;

    int len = strlen(input);
    char* result = new char[len + 1]; // +1 for null terminator
    int j = 0;

    for (int i = 0; i < len; i++)
    {
        if (input[i] != ' ')
        {
            result[j++] = input[i];
        }
    }
    result[j] = '\0'; // Null terminate

    return result;
}

char* BuildPath(const char* dir, const char* filename)
{
    int len1 = strlen(dir);
    int len2 = strlen(filename);
    int len3 = strlen(".bmp");

    char* result = new char[len1 + len2 + len3 + 1]; // +1 for null terminator
    strcpy_s(result, len1 + len2 + len3 + 1, dir);
    strcat_s(result, len1 + len2 + len3 + 1, filename);
    strcat_s(result, len1 + len2 + len3 + 1, ".bmp");

    return result;
}

char* CreateStringCopy(const char* input)
{
    if (!input) return nullptr;

    int len = strlen(input);
    char* result = new char[len + 1];
    strcpy_s(result, len + 1, input);
    return result;
}

int ParseWeaponList(const char* weaponStr, const char* weapons[], int maxWeapons)
{
    if (!weaponStr || !weapons || maxWeapons <= 0) return 0;

    int weaponCount = 0;
    const char* ptr = weaponStr;

    while (*ptr && weaponCount < maxWeapons)
    {
        // Skip leading spaces
        while (*ptr == ' ') ++ptr;

        // Save start of this weapon name
        const char* start = ptr;

        // Find end of weapon name (comma or end of string)
        while (*ptr && *ptr != ',') ++ptr;

        // Calculate length of this weapon name
        int len = (int)(ptr - start);

        // If we found a non-empty name, store it
        if (len > 0)
        {
            // Allocate memory for the weapon name
            char* weaponName = new char[len + 1];
            strncpy_s(weaponName, len + 1, start, len);

            // Trim trailing spaces
            while (len > 0 && weaponName[len - 1] == ' ')
                weaponName[--len] = '\0';

            // Store in our array if it's not empty
            if (len > 0)
                weapons[weaponCount++] = weaponName;
            else
                delete[] weaponName;
        }

        // Skip comma if present
        if (*ptr == ',') ++ptr;
    }

    return weaponCount;
}

void FreeWeaponList(const char* weapons[], int count)
{
    if (!weapons) return;

    for (int i = 0; i < count; i++)
    {
        if (weapons[i])
        {
            delete[] (char*)weapons[i];
        }
    }
}

} // namespace StringUtils
