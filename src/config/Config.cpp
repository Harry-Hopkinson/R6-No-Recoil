#include "Config.hpp"

#include <windows.h>
#include "../Globals.hpp"
#include "../recoil/Recoil.hpp"

#include "../core/utils/clamp.hpp"

void SaveConfig()
{
    HANDLE file = CreateFileA("Config.toml", GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) return;

    char buffer[1024];
    int len = 0;

    len += wsprintfA(buffer + len, "[RecoilPresets]\r\n");
    len += wsprintfA(buffer + len, "# 0 = LOW, 1 = MEDIUM, 2 = HIGH, 3 = ULTRA\r\n");
    len += wsprintfA(buffer + len, "Mode = %d\r\n", SelectedMode);
    len += wsprintfA(buffer + len, "Enabled = %s\r\n", EnableRC ? "true" : "false");
    len += wsprintfA(buffer + len, "\r\n");

    len += wsprintfA(buffer + len, "# Vertical recoil amounts for each preset\r\n");
    len += wsprintfA(buffer + len, "LowSensVertical = %d\r\n", RecoilPresets[0].Vertical);
    len += wsprintfA(buffer + len, "MediumSensVertical = %d\r\n", RecoilPresets[1].Vertical);
    len += wsprintfA(buffer + len, "HighSensVertical = %d\r\n", RecoilPresets[2].Vertical);
    len += wsprintfA(buffer + len, "UltraSensVertical = %d\r\n", RecoilPresets[3].Vertical);
    len += wsprintfA(buffer + len, "\r\n");

    len += wsprintfA(buffer + len, "# Horizontal recoil amounts for each preset\r\n");
    len += wsprintfA(buffer + len, "LowSensHorizontal = %d\r\n", RecoilPresets[0].Horizontal);
    len += wsprintfA(buffer + len, "MediumSensHorizontal = %d\r\n", RecoilPresets[1].Horizontal);
    len += wsprintfA(buffer + len, "HighSensHorizontal = %d\r\n", RecoilPresets[2].Horizontal);
    len += wsprintfA(buffer + len, "UltraSensHorizontal = %d\r\n", RecoilPresets[3].Horizontal);
    len += wsprintfA(buffer + len, "\r\n");

    len += wsprintfA(buffer + len, "# Use https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes\n");
    len += wsprintfA(buffer + len, "# And then convert the key code to decimal\n");
    len += wsprintfA(buffer + len, "# Toggle Key (e.g. 20 = CAPS LOCK)\r\n");
    len += wsprintfA(buffer + len, "ToggleKey = %d\r\n\r\n", ToggleKey);

    DWORD written;
    WriteFile(file, buffer, len, &written, NULL);
    CloseHandle(file);
}

void LoadConfig()
{
    HANDLE file = CreateFileA("Config.toml", GENERIC_READ, FILE_SHARE_READ, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        SaveConfig();  // Create default config file
        return;
    }

    DWORD fileSize = GetFileSize(file, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0)
    {
        CloseHandle(file);
        return;
    }

    char* buffer = new char[fileSize + 1];
    DWORD bytesRead;
    if (!ReadFile(file, buffer, fileSize, &bytesRead, NULL))
    {
        delete[] buffer;
        CloseHandle(file);
        return;
    }

    buffer[bytesRead] = '\0';
    CloseHandle(file);

    char section[32] = {0};
    char* line = strtok(buffer, "\r\n");
    while (line)
    {
        while (*line == ' ' || *line == '\t') ++line;
        if (*line == '\0' || *line == '#')
        {
            line = strtok(NULL, "\r\n");
            continue;
        }

        if (line[0] == '[')
        {
            char* end = strchr(line, ']');
            if (end && static_cast<size_t>(end - line - 1) < sizeof(section))
            {
                strncpy(section, line + 1, end - line - 1);
                section[end - line - 1] = '\0';
            }
            line = strtok(NULL, "\r\n");
            continue;
        }

        char* equal = strchr(line, '=');
        if (!equal)
        {
            line = strtok(NULL, "\r\n");
            continue;
        }

        *equal = '\0';
        char* key = line;
        char* value = equal + 1;

        // Trim whitespace
        while (*key == ' ' || *key == '\t') ++key;
        char* keyEnd = key + strlen(key) - 1;
        while (keyEnd > key && (*keyEnd == ' ' || *keyEnd == '\t')) *keyEnd-- = '\0';

        while (*value == ' ' || *value == '\t') ++value;
        char* valueEnd = value + strlen(value) - 1;
        while (valueEnd > value && (*valueEnd == ' ' || *valueEnd == '\t')) *valueEnd-- = '\0';

        if (strcmp(section, "RecoilPresets") == 0)
        {
            if (strcmp(key, "Mode") == 0) SelectedMode = atoi(value);
            else if (strcmp(key, "Enabled") == 0) EnableRC = (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
            else if (strcmp(key, "LowSensVertical") == 0) RecoilPresets[0].Vertical = atoi(value);
            else if (strcmp(key, "LowSensHorizontal") == 0) RecoilPresets[0].Horizontal = atoi(value);
            else if (strcmp(key, "MediumSensVertical") == 0) RecoilPresets[1].Vertical = atoi(value);
            else if (strcmp(key, "MediumSensHorizontal") == 0) RecoilPresets[1].Horizontal = atoi(value);
            else if (strcmp(key, "HighSensVertical") == 0) RecoilPresets[2].Vertical = atoi(value);
            else if (strcmp(key, "HighSensHorizontal") == 0) RecoilPresets[2].Horizontal = atoi(value);
            else if (strcmp(key, "UltraSensVertical") == 0) RecoilPresets[3].Vertical = atoi(value);
            else if (strcmp(key, "UltraSensHorizontal") == 0) RecoilPresets[3].Horizontal = atoi(value);
            else if (strcmp(key, "ToggleKey") == 0) ToggleKey = atoi(value);
        }

        line = strtok(NULL, "\r\n");
    }

    delete[] buffer;

    SelectedMode = clamp(SelectedMode, 0, 3);
    CurrentRecoil = RecoilPresets[SelectedMode];
}
