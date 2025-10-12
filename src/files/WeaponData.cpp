#include "Files.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "../recoil/Recoil.h"

#include <windows.h>

extern "C"
{
    #include "../json/cJSON.h"
}

namespace Files
{
    WeaponRecoil GetWeaponData(const char* weaponName)
    {
        WeaponRecoil recoil = {3, 0};
        if (!weaponName) return recoil;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file) {
            MessageBoxA(NULL, "Failed to open WeaponData.json", "Error", MB_OK);
            return recoil;
        }

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        cJSON* root = cJSON_Parse(data);
        free(data);
        if (!root) {
            MessageBoxA(NULL, "Failed to parse JSON", "Error", MB_OK);
            return recoil;
        }

        int nonMagVert = 0, nonMagHorz = 0;
        int magVert = 0, magHorz = 0;

        cJSON* weapon = NULL;
        cJSON_ArrayForEach(weapon, root)
        {
            cJSON* name = cJSON_GetObjectItem(weapon, "name");
            if (!name || strcmp(name->valuestring, weaponName) != 0)
                continue;

            cJSON* recoilObj = cJSON_GetObjectItem(weapon, "recoil");
            if (!recoilObj) break;

            cJSON* nonMag = cJSON_GetObjectItem(recoilObj, "non_magnified");
            if (nonMag) {
                nonMagVert = cJSON_GetObjectItem(nonMag, "vertical") ? cJSON_GetObjectItem(nonMag, "vertical")->valueint : 0;
                nonMagHorz = cJSON_GetObjectItem(nonMag, "horizontal") ? cJSON_GetObjectItem(nonMag, "horizontal")->valueint : 0;
            }

            cJSON* mag = cJSON_GetObjectItem(recoilObj, "magnified");
            if (mag) {
                magVert = cJSON_GetObjectItem(mag, "vertical") ? cJSON_GetObjectItem(mag, "vertical")->valueint : 0;
                magHorz = cJSON_GetObjectItem(mag, "horizontal") ? cJSON_GetObjectItem(mag, "horizontal")->valueint : 0;
            }

            break; // found weapon
        }

        cJSON_Delete(root);

        if (SelectedScopeType == ScopeType::MAGNIFIED)
            recoil = { magVert != 0 || magHorz != 0 ? magVert : nonMagVert,
                       magVert != 0 || magHorz != 0 ? magHorz : nonMagHorz };
        else
            recoil = { nonMagVert, nonMagHorz };

        if (recoil.Vertical == 0 && recoil.Horizontal == 0)
            recoil = {3, 0};

        // Debug popup
        char msg[256];
        sprintf(msg, "Weapon: %s\nNM: %d,%d\nM: %d,%d\nResult: %d,%d",
                weaponName, nonMagVert, nonMagHorz, magVert, magHorz,
                recoil.Vertical, recoil.Horizontal);
        MessageBoxA(NULL, msg, "Recoil Debug", MB_OK);

        return recoil;
    }
}
