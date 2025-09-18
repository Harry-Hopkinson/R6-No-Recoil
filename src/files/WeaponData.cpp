#include "Files.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Files
{

    WeaponRecoil GetWeaponData(const char* weaponName)
    {
        WeaponRecoil recoil = { 3, 0 };
        if (!weaponName)
            return recoil; // Default medium recoil

        FILE* file = fopen("WeaponData.json", "r");
        if (!file)
            return recoil; // Failed to open file

        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            char* namePos = strstr(line, "\"name\"");
            if (namePos)
            {
                char* quote1 = strchr(namePos, '"');
                if (!quote1)
                    continue;
                char* quote2 = strchr(quote1 + 1, '"');
                if (!quote2)
                    continue;
                char* quote3 = strchr(quote2 + 1, '"');
                if (!quote3)
                    continue;
                char* quote4 = strchr(quote3 + 1, '"');
                if (!quote4)
                    continue;

                ptrdiff_t len = quote4 - quote3 - 1;
                if (len == (ptrdiff_t)strlen(weaponName) && strncmp(weaponName, quote3 + 1, len) == 0)
                {
                    // Find vertical
                    char* verticalPos = strstr(line, "\"vertical\"");
                    if (verticalPos)
                    {
                        char* colon = strchr(verticalPos, ':');
                        if (colon)
                            recoil.Vertical = atoi(colon + 1);
                    }
                    // Find horizontal
                    char* horizontalPos = strstr(line, "\"horizontal\"");
                    if (horizontalPos)
                    {
                        char* colon = strchr(horizontalPos, ':');
                        if (colon)
                            recoil.Horizontal = atoi(colon + 1);
                    }
                    fclose(file);
                    return recoil;
                }
            }
        }

        fclose(file);
        return recoil; // Default if not found
    }

} // namespace Files
