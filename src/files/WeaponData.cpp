#include "Files.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../recoil/Structs.h"

namespace Files
{
    WeaponRecoil GetWeaponData(const char* weaponName)
    {
        // Default recoil
        WeaponRecoil recoil = {3, 0};
        if (!weaponName) return recoil;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file) return recoil;

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        char* pos = data;

        int nonMagAngledVert = 0, nonMagAngledHorz = 0;
        int nonMagHorizontalVert = 0, nonMagHorizontalHorz = 0;
        int nonMagVerticalVert = 0, nonMagVerticalHorz = 0;

        int magAngledVert = 0, magAngledHorz = 0;
        int magHorizontalVert = 0, magHorizontalHorz = 0;
        int magVerticalVert = 0, magVerticalHorz = 0;

        while ((pos = strstr(pos, "\"name\"")))
        {
            char name[128] = {0};
            if (sscanf(pos, " \"name\" : \"%127[^\"]\"", name) == 1)
            {
                if (strcmp(name, weaponName) != 0)
                {
                    pos += strlen(name);
                    continue;
                }

                char* recoilPos = strstr(pos, "\"recoil\"");
                if (!recoilPos) break;

                // Non-magnified
                char* nonMagPos = strstr(recoilPos, "\"non_magnified\"");
                if (nonMagPos)
                {
                    sscanf(strstr(nonMagPos, "\"angled\""), " \"angled\" : { \"vertical\" : %d, \"horizontal\" : %d }",
                           &nonMagAngledVert, &nonMagAngledHorz);
                    sscanf(strstr(nonMagPos, "\"horizontal\""), " \"horizontal\" : { \"vertical\" : %d, \"horizontal\" : %d }",
                           &nonMagHorizontalVert, &nonMagHorizontalHorz);
                    sscanf(strstr(nonMagPos, "\"vertical\""), " \"vertical\" : { \"vertical\" : %d, \"horizontal\" : %d }",
                           &nonMagVerticalVert, &nonMagVerticalHorz);
                }

                // Magnified
                char* magPos = strstr(recoilPos, "\"magnified\"");
                if (magPos)
                {
                    sscanf(strstr(magPos, "\"angled\""), " \"angled\" : { \"vertical\" : %d, \"horizontal\" : %d }",
                           &magAngledVert, &magAngledHorz);
                    sscanf(strstr(magPos, "\"horizontal\""), " \"horizontal\" : { \"vertical\" : %d, \"horizontal\" : %d }",
                           &magHorizontalVert, &magHorizontalHorz);
                    sscanf(strstr(magPos, "\"vertical\""), " \"vertical\" : { \"vertical\" : %d, \"horizontal\" : %d }",
                           &magVerticalVert, &magVerticalHorz);
                }

                break;
            }
            pos += 6;
        }

        free(data);

        // Select the correct recoil based on grip & scope
        if (SelectedScopeType == ScopeType::MAGNIFIED)
        {
            switch (SelectedGripType)
            {
                case GripType::ANGLED: recoil = { magAngledVert, magAngledHorz }; break;
                case GripType::HORIZONTAL: recoil = { magHorizontalVert, magHorizontalHorz }; break;
                case GripType::VERTICAL: recoil = { magVerticalVert, magVerticalHorz }; break;
                default: recoil = { magVerticalVert, magVerticalHorz }; break;
            }
        }
        else
        {
            switch (SelectedGripType)
            {
                case GripType::ANGLED: recoil = { nonMagAngledVert, nonMagAngledHorz }; break;
                case GripType::HORIZONTAL: recoil = { nonMagHorizontalVert, nonMagHorizontalHorz }; break;
                case GripType::VERTICAL: recoil = { nonMagVerticalVert, nonMagVerticalHorz }; break;
                default: recoil = { nonMagVerticalVert, nonMagVerticalHorz }; break;
            }
        }

        // Fallback if all zeros
        if (recoil.Vertical == 0 && recoil.Horizontal == 0)
            recoil = {3, 0};

        return recoil;
    }
}
