#include "Files.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace Files
{

    int GetWeaponData(const char* weaponName)
    {
        if (!weaponName)
            return 3; // Default medium recoil

        FILE* file = fopen("WeaponData.json", "r");
        if (!file)
            return 3; // Failed to open file

        char line[128];
        while (fgets(line, sizeof(line), file))
        {
            char* namePos = strstr(line, "\"name\"");
            if (namePos)
            {
                char* quote1 = strchr(namePos, '"');
                if (!quote1) continue;
                char* quote2 = strchr(quote1 + 1, '"');
                if (!quote2) continue;
                char* quote3 = strchr(quote2 + 1, '"');
                if (!quote3) continue;
                char* quote4 = strchr(quote3 + 1, '"');
                if (!quote4) continue;

                ptrdiff_t len = quote4 - quote3 - 1;
                if (len == (ptrdiff_t)strlen(weaponName) &&
                    strncmp(weaponName, quote3 + 1, len) == 0)
                {
                    char* recoilPos = strstr(line, "\"recoil\"");
                    if (recoilPos)
                    {
                        char* colon = strchr(recoilPos, ':');
                        if (colon)
                        {
                            int recoil = atoi(colon + 1);
                            fclose(file);
                            return recoil;
                        }
                    }
                }
            }
        }

        fclose(file);
        return 3; // Default if not found
    }

} // namespace Files
