#include <cstdio>
#include <cstdlib>
#include <cstring>

enum class ScopeType
{
    NONE,
    MAGNIFIED,
    NON_MAGNIFIED
};

enum class GripType
{
    NONE,
    ANGLED,
    HORIZONTAL,
    VERTICAL
};

namespace Files
{
    void SaveWeaponRecoil(const char* weaponName, float vert, float hor)
    {
        ScopeType SelectedScopeType = ScopeType::NON_MAGNIFIED;
        GripType SelectedGripType = GripType::HORIZONTAL;

        if (!weaponName)
            return;

        FILE* file = fopen("WeaponData.json", "rb");
        if (!file)
            return;

        fseek(file, 0, SEEK_END);
        long len = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* data = (char*)malloc(len + 1);
        fread(data, 1, len, file);
        data[len] = '\0';
        fclose(file);

        // Find weapon name
        char* weaponPos = strstr(data, weaponName);
        if (!weaponPos)
        {
            free(data);
            return;
        }

        // Find scope block
        const char* scopeBlock = (SelectedScopeType == ScopeType::MAGNIFIED) ? "\"magnified\"" : "\"non_magnified\"";
        char* scopePos = strstr(weaponPos, scopeBlock);
        if (!scopePos)
        {
            free(data);
            return;
        }

        // Grip block
        const char* gripBlock = nullptr;
        switch (SelectedGripType)
        {
            case GripType::ANGLED:
                gripBlock = "\"angled\"";
                break;
            case GripType::HORIZONTAL:
                gripBlock = "\"horizontal\"";
                break;
            case GripType::VERTICAL:
                gripBlock = "\"vertical\"";
                break;
            default:
                gripBlock = "\"vertical\"";
                break;
        }

        char* gripPos = strstr(scopePos, gripBlock);
        if (!gripPos)
        {
            free(data);
            return;
        }

        // Find braces for this grip block
        char* braceStart = strchr(gripPos, '{');
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

        // Reconstruct JSON with corrected block
        size_t beforeLen = braceStart - data + 1; // include '{'
        size_t afterLen = strlen(braceEnd);
        char* before = (char*)malloc(beforeLen + 1);
        memcpy(before, data, beforeLen);
        before[beforeLen] = '\0';

        // ✅ Properly close the brace here
        char newBlock[256];
        snprintf(newBlock, sizeof(newBlock), " \"vertical\": %.2f, \"horizontal\": %.2f }", vert, hor);

        size_t newSize = beforeLen + strlen(newBlock) + afterLen + 2;
        char* newData = (char*)malloc(newSize);
        snprintf(newData, newSize, "%s%s%s", before, newBlock, braceEnd);

        // Write to file
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

int main()
{
    Files::SaveWeaponRecoil("P90", 3.5f, 0.5f);
    return 0;
}
