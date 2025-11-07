#include "Files.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../Globals.h"
#include "../recoil/Recoil.h"

namespace Files
{

    WeaponRecoil GetWeaponData(const char* weaponName, int presetIndex)
    {
        WeaponRecoil recoil = { 3.0f, 0.0f };
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

                // Find preset key (preset-1, preset-2, etc.)
                char presetKey[32];
                snprintf(presetKey, sizeof(presetKey), "\"preset-%d\"", presetIndex);

                char* presetPos = strstr(pos, presetKey);
                if (!presetPos) break;

                float vertical = 0.0f, horizontal = 0.0f;
                sscanf(presetPos, " \"%*[^\"]\" : { \"vertical\" : %f , \"horizontal\" : %f", &vertical, &horizontal);

                recoil.Vertical = vertical;
                recoil.Horizontal = horizontal;
                break;
            }
            pos += 6;
        }

        free(data);

        return recoil;
    }

    void SaveWeaponData(int presetIndex)
    {
        if (!CurrentWeapon) return;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file) return;

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        // Find weapon name
        char* weaponPos = strstr(data, CurrentWeapon);
        if (!weaponPos)
        {
            free(data);
            return;
        }

        // Find preset block
        char presetKey[32];
        snprintf(presetKey, sizeof(presetKey), "\"preset-%d\"", presetIndex);
        char* presetPos = strstr(weaponPos, presetKey);
        if (!presetPos)
        {
            free(data);
            return;
        }

        // Find braces for this preset block
        char* braceStart = strchr(presetPos, '{');
        if (!braceStart)
        {
            free(data);
            return;
        }

        int depth = 1;
        char* braceEnd = braceStart + 1;
        while (*braceEnd && depth > 0)
        {
            if (*braceEnd == '{')
                depth++;
            else if (*braceEnd == '}')
                depth--;
            braceEnd++;
        }

        // Rebuild JSON block
        size_t beforeLen = braceStart - data + 1;
        size_t afterLen = strlen(braceEnd);
        char* before = (char*)malloc(beforeLen + 1);
        memcpy(before, data, beforeLen);
        before[beforeLen] = '\0';

        char newBlock[256];
        snprintf(
            newBlock, sizeof(newBlock), " \"vertical\": %.1f, \"horizontal\": %.1f }", CurrentRecoil.Vertical,
            CurrentRecoil.Horizontal);

        size_t newSize = beforeLen + strlen(newBlock) + afterLen + 2;
        char* newData = (char*)malloc(newSize);
        snprintf(newData, newSize, "%s%s%s", before, newBlock, braceEnd);

        // Write updated JSON
        file = fopen("WeaponData.json", "wb");
        if (file)
        {
            fwrite(newData, 1, strlen(newData), file);
            fclose(file);
        }

        free(before);
        free(newData);
        free(data);
    }

} // namespace Files
