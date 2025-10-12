#include "Files.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../recoil/Recoil.h"

namespace Files
{
    WeaponRecoil GetWeaponData(const char* weaponName)
    {
        WeaponRecoil recoil = {3, 0};
        if (!weaponName) return recoil;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file) {
            return recoil;
        }

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        char* pos = data;
        int nonMagVert = 0, nonMagHorz = 0;
        int magVert = 0, magHorz = 0;

        while ((pos = strstr(pos, "\"name\""))) {
            char name[128] = {0};
            if (sscanf(pos, " \"name\" : \"%127[^\"]\"", name) == 1) {
                if (strcmp(name, weaponName) != 0) {
                    pos += strlen(name);
                    continue;
                }

                // Found weapon, now find its recoil section
                char* recoilPos = strstr(pos, "\"recoil\"");
                if (!recoilPos) break;

                // --- Non-magnified ---
                char* nonMagPos = strstr(recoilPos, "\"non_magnified\"");
                if (nonMagPos) {
                    char* vertPos = strstr(nonMagPos, "\"vertical\"");
                    if (vertPos) sscanf(vertPos, " \"vertical\" : %d", &nonMagVert);

                    char* horPos = strstr(nonMagPos, "\"horizontal\"");
                    if (horPos) sscanf(horPos, " \"horizontal\" : %d", &nonMagHorz);
                }

                // --- Magnified ---
                char* magPos = strstr(recoilPos, "\"magnified\"");
                if (magPos) {
                    char* vertPos = strstr(magPos, "\"vertical\"");
                    if (vertPos) sscanf(vertPos, " \"vertical\" : %d", &magVert);

                    char* horPos = strstr(magPos, "\"horizontal\"");
                    if (horPos) sscanf(horPos, " \"horizontal\" : %d", &magHorz);
                }

                break; // weapon found
            }
            pos += 6; // move past "name"
        }

        free(data);

        if (SelectedScopeType == ScopeType::MAGNIFIED)
            recoil = { magVert != 0 || magHorz != 0 ? magVert : nonMagVert,
                       magVert != 0 || magHorz != 0 ? magHorz : nonMagHorz };
        else
            recoil = { nonMagVert, nonMagHorz };

        if (recoil.Vertical == 0 && recoil.Horizontal == 0)
            recoil = {3, 0};

        return recoil;
    }
}
