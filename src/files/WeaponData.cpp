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
            if (strstr(line, "\"name\"") && strstr(line, weaponName))
            {
                while (fgets(line, sizeof(line), file))
                {
                    char* pos = strstr(line, "\"recoil\"");
                    if (pos)
                    {
                        char* colon = strchr(pos, ':');
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