#include "Files.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../recoil/Recoil.h"

namespace Files
{

WeaponRecoil GetWeaponData(const char* weaponName)
{
    WeaponRecoil recoil = { 3, 0 }; // Default medium recoil
    if (!weaponName)
        return recoil;

    FILE* file = fopen("WeaponData.json", "r");
    if (!file)
        return recoil;

    char line[512];
    bool insideWeapon = false;
    ScopeType currentScope = ScopeType::NONE;

    while (fgets(line, sizeof(line), file))
    {
        char* trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t')
            ++trimmed;

        // Check for weapon name
        if (strstr(trimmed, "\"name\""))
        {
            char* quote1 = strchr(trimmed, '"');
            if (!quote1) continue;
            char* quote2 = strchr(quote1 + 1, '"');
            if (!quote2) continue;
            char* quote3 = strchr(quote2 + 1, '"');
            if (!quote3) continue;
            char* quote4 = strchr(quote3 + 1, '"');
            if (!quote4) continue;

            ptrdiff_t len = quote4 - quote3 - 1;
            if (len == (ptrdiff_t)strlen(weaponName) && strncmp(weaponName, quote3 + 1, len) == 0) insideWeapon = true;
            else insideWeapon = false;
        }

        if (!insideWeapon)
            continue;

        // Determine which scope block we are in
        if (strstr(trimmed, "\"non_magnified\"")) currentScope = ScopeType::NON_MAGNIFIED;
        if (strstr(trimmed, "\"magnified\"")) currentScope = ScopeType::MAGNIFIED;

        if (currentScope != SelectedScopeType)
            continue; // Skip blocks not matching the selected scope

        // Parse vertical
        if (strstr(trimmed, "\"vertical\""))
        {
            char* colon = strchr(trimmed, ':');
            if (colon)
                recoil.Vertical = atoi(colon + 1);
        }

        // Parse horizontal
        if (strstr(trimmed, "\"horizontal\""))
        {
            char* colon = strchr(trimmed, ':');
            if (colon)
                recoil.Horizontal = atoi(colon + 1);
        }

        // If both vertical and horizontal found, we can return
        if (strstr(trimmed, "}"))
        {
            fclose(file);
            return recoil;
        }
    }

    fclose(file);
    return recoil; // Default if not found
}

} // namespace Files
