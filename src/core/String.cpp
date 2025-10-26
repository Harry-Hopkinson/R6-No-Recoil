#include "String.h"

#include "../utils/StringUtils.h"

namespace String
{

    char* BuildPath(const char* dir, const char* filename)
    {
        return StringUtils::BuildPath(dir, filename, ".bmp");
    }

    char* CreateStringCopy(const char* input)
    {
        return StringUtils::CopyString(input);
    }

    int ParseWeaponList(const char* weaponStr, const char* weapons[], int maxWeapons)
    {
        return StringUtils::ParseWeaponList(weaponStr, weapons, maxWeapons);
    }

    void FreeWeaponList(const char* weapons[], int count)
    {
        StringUtils::FreeWeaponList(weapons, count);
    }

} // namespace String
