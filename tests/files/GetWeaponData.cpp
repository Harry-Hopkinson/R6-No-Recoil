#include "Structs.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

struct WeaponRecoil
{
    float Vertical;
    float Horizontal;
};

namespace Files
{

    void ParseRecoilBlock(const char* gripBlockStart, float& vert, float& hor)
    {
        // Find the opening brace immediately after the grip key
        const char* braceStart = strchr(gripBlockStart, '{');
        if (!braceStart)
            return;

        // Find the matching closing brace
        const char* braceEnd = braceStart;
        int depth = 1;
        while (*++braceEnd && depth > 0)
        {
            if (*braceEnd == '{')
                depth++;
            else if (*braceEnd == '}')
                depth--;
        }
        if (depth != 0)
            return;

        // Parse only inside this brace
        const char* p = braceStart + 1;
        while (p < braceEnd)
        {
            if (strncmp(p, "\"vertical\"", 10) == 0)
            {
                const char* colon = strchr(p, ':');
                if (colon)
                    vert = strtof(colon + 1, nullptr);
            }
            else if (strncmp(p, "\"horizontal\"", 12) == 0)
            {
                const char* colon = strchr(p, ':');
                if (colon)
                    hor = strtof(colon + 1, nullptr);
            }
            p++;
        }
    }

    WeaponRecoil GetWeaponData(const char* weaponName)
    {
        WeaponRecoil recoil = { 3.0f, 0.0f };
        if (!weaponName)
            return recoil;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file)
            return recoil;

        ScopeType SelectedScopeType = ScopeType::NON_MAGNIFIED;
        GripType SelectedGripType = GripType::HORIZONTAL;

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        char* pos = data;

        // Non-magnified
        float nonMagAngledVert = 0.0f, nonMagAngledHorz = 0.0f;
        float nonMagHorizontalVert = 0.0f, nonMagHorizontalHorz = 0.0f;
        float nonMagVerticalVert = 0.0f, nonMagVerticalHorz = 0.0f;

        // Magnified
        float magAngledVert = 0.0f, magAngledHorz = 0.0f;
        float magHorizontalVert = 0.0f, magHorizontalHorz = 0.0f;
        float magVerticalVert = 0.0f, magVerticalHorz = 0.0f;

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
                    char* horizontalKey = strstr(nonMagPos, "\"horizontal\"");
                    if (horizontalKey)
                        ParseRecoilBlock(horizontalKey, nonMagHorizontalVert, nonMagHorizontalHorz);

                    char* verticalKey = strstr(nonMagPos, "\"vertical\"");
                    if (verticalKey)
                        ParseRecoilBlock(verticalKey, nonMagVerticalVert, nonMagVerticalHorz);

                    char* angledKey = strstr(nonMagPos, "\"angled\"");
                    if (angledKey)
                        ParseRecoilBlock(angledKey, nonMagAngledVert, nonMagAngledHorz);
                }

                // --- Magnified ---
                char* magPos = strstr(recoilPos, "\"magnified\"");
                if (magPos)
                {
                    char* horizontalKey = strstr(magPos, "\"horizontal\"");
                    if (horizontalKey)
                        ParseRecoilBlock(horizontalKey, magHorizontalVert, magHorizontalHorz);

                    char* verticalKey = strstr(magPos, "\"vertical\"");
                    if (verticalKey)
                        ParseRecoilBlock(verticalKey, magVerticalVert, magVerticalHorz);

                    char* angledKey = strstr(magPos, "\"angled\"");
                    if (angledKey)
                        ParseRecoilBlock(angledKey, magAngledVert, magAngledHorz);
                }

                break;
            }
            pos += 6;
        }

        free(data);

        // Select values based on scope and grip
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

        if (recoil.Vertical == 0.0f && recoil.Horizontal == 0.0f)
            recoil = { 3.0f, 0.0f };

        return recoil;
    }

} // namespace Files

int main()
{
    const char* weaponName = "P90";
    WeaponRecoil recoil = Files::GetWeaponData(weaponName);
    printf("Weapon: %s\n", weaponName);
    printf("ScopeType: NON_MAGNIFIED\nGripType: HORIZONTAL\n");
    printf("Vertical Recoil: %.2f\nHorizontal Recoil: %.2f\n", recoil.Vertical, recoil.Horizontal);
    return 0;
}
