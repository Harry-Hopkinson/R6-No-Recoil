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
            return recoil;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file)
            return recoil;

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        char* pos = data;

        // Non-magnified
        int nonMagAngledVert = 0, nonMagAngledHorz = 0;
        int nonMagHorizontalVert = 0, nonMagHorizontalHorz = 0;
        int nonMagVerticalVert = 0, nonMagVerticalHorz = 0;

        // Magnified
        int magAngledVert = 0, magAngledHorz = 0;
        int magHorizontalVert = 0, magHorizontalHorz = 0;
        int magVerticalVert = 0, magVerticalHorz = 0;

        while ((pos = strstr(pos, "\"name\"")))
        {
            char name[128] = { 0 };
            if (sscanf(pos, " \"name\" : \"%127[^\"]\"", name) == 1)
            {
                if (strcmp(name, weaponName) != 0)
                {
                    pos += strlen(name);
                    continue;
                }

                char* recoilPos = strstr(pos, "\"recoil\"");
                if (!recoilPos)
                    break;

                // --- Non-magnified ---
                char* nonMagPos = strstr(recoilPos, "\"non_magnified\"");
                if (nonMagPos)
                {
                    // Angled
                    char* angledPos = strstr(nonMagPos, "\"angled\"");
                    if (angledPos)
                    {
                        char* braceStart = strchr(angledPos, '{');
                        if (braceStart)
                            sscanf(
                                braceStart, "{ \"vertical\" : %d, \"horizontal\" : %d }", &nonMagAngledVert, &nonMagAngledHorz);
                    }

                    // Horizontal
                    char* horizontalPos = strstr(nonMagPos, "\"horizontal\"");
                    if (horizontalPos)
                    {
                        char* braceStart = strchr(horizontalPos, '{');
                        if (braceStart)
                            sscanf(
                                braceStart, "{ \"vertical\" : %d, \"horizontal\" : %d }", &nonMagHorizontalVert,
                                &nonMagHorizontalHorz);
                    }

                    // Vertical
                    char* verticalPos = strstr(nonMagPos, "\"vertical\"");
                    if (verticalPos)
                    {
                        char* braceStart = strchr(verticalPos, '{');
                        if (braceStart)
                            sscanf(
                                braceStart, "{ \"vertical\" : %d, \"horizontal\" : %d }", &nonMagVerticalVert,
                                &nonMagVerticalHorz);
                    }
                }

                // --- Magnified ---
                char* magPos = strstr(recoilPos, "\"magnified\"");
                if (magPos)
                {
                    // Angled
                    char* angledPos = strstr(magPos, "\"angled\"");
                    if (angledPos)
                    {
                        char* braceStart = strchr(angledPos, '{');
                        if (braceStart)
                            sscanf(braceStart, "{ \"vertical\" : %d, \"horizontal\" : %d }", &magAngledVert, &magAngledHorz);
                    }

                    // Horizontal
                    char* horizontalPos = strstr(magPos, "\"horizontal\"");
                    if (horizontalPos)
                    {
                        char* braceStart = strchr(horizontalPos, '{');
                        if (braceStart)
                            sscanf(
                                braceStart, "{ \"vertical\" : %d, \"horizontal\" : %d }", &magHorizontalVert,
                                &magHorizontalHorz);
                    }

                    // Vertical
                    char* verticalPos = strstr(magPos, "\"vertical\"");
                    if (verticalPos)
                    {
                        char* braceStart = strchr(verticalPos, '{');
                        if (braceStart)
                            sscanf(
                                braceStart, "{ \"vertical\" : %d, \"horizontal\" : %d }", &magVerticalVert, &magVerticalHorz);
                    }
                }

                break; // Weapon found
            }
            pos += 6;
        }

        free(data);

        // --- Select recoil based on grip & scope ---
        if (SelectedScopeType == ScopeType::MAGNIFIED)
        {
            switch (SelectedGripType)
            {
                case GripType::ANGLED:
                    recoil = { magAngledVert, magAngledHorz };
                    break;
                case GripType::HORIZONTAL:
                    recoil = { magHorizontalVert, magHorizontalHorz };
                    break;
                case GripType::VERTICAL:
                    recoil = { magVerticalVert, magVerticalHorz };
                    break;
                default:
                    recoil = { magVerticalVert, magVerticalHorz };
                    break;
            }
        }
        else
        {
            switch (SelectedGripType)
            {
                case GripType::ANGLED:
                    recoil = { nonMagAngledVert, nonMagAngledHorz };
                    break;
                case GripType::HORIZONTAL:
                    recoil = { nonMagHorizontalVert, nonMagHorizontalHorz };
                    break;
                case GripType::VERTICAL:
                    recoil = { nonMagVerticalVert, nonMagVerticalHorz };
                    break;
                default:
                    recoil = { nonMagVerticalVert, nonMagVerticalHorz };
                    break;
            }
        }

        // --- Fallback if zero ---
        if (recoil.Vertical == 0 && recoil.Horizontal == 0)
            recoil = { 3, 0 };

        return recoil;
    }

} // namespace Files
