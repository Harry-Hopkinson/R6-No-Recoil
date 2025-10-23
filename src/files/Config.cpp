#include "Files.h"

#include "../Globals.h"
#include "../recoil/Recoil.h"

#include <cstdio>

#include <windows.h>

namespace Files
{

    void SaveConfig()
    {
        HANDLE file = CreateFileA("Config.toml", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE)
            return;

        char buffer[512];
        int len = 0;
        const int bufferSize = sizeof(buffer);

        // [RecoilPresets]
        len += sprintf_s(buffer + len, bufferSize - len, "[RecoilPresets]\r\n");
        len += sprintf_s(buffer + len, bufferSize - len, "Enabled = %s\r\n", EnableRC ? "true" : "false");
        len += sprintf_s(buffer + len, bufferSize - len, "VerticalRecoil = %.1f\r\n", CurrentRecoil.Vertical);
        len += sprintf_s(buffer + len, bufferSize - len, "HorizontalRecoil = %.1f\r\n", CurrentRecoil.Horizontal);
        len += sprintf_s(buffer + len, bufferSize - len, "\r\n");


        // [Controller]
        len += sprintf_s(buffer + len, bufferSize - len, "[Controller]\r\n");
        len += sprintf_s(buffer + len, bufferSize - len, "Enabled = %s\r\n", EnableController ? "true" : "false");
        len += sprintf_s(buffer + len, bufferSize - len, "Multiplier = %d\r\n", ControllerMultiplier);
        len += sprintf_s(buffer + len, bufferSize - len, "\r\n");

        // [ToggleKey]
        len += sprintf_s(buffer + len, bufferSize - len, "[ToggleKey]\r\n");
        len += sprintf_s(buffer + len, bufferSize - len,
                         "# Use https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes\r\n");
        len += sprintf_s(buffer + len, bufferSize - len, "# And then convert the key code to decimal\r\n");
        len += sprintf_s(buffer + len, bufferSize - len, "# Toggle Key (e.g. 20 = CAPS LOCK)\r\n");
        len += sprintf_s(buffer + len, bufferSize - len, "ToggleKey = %d\r\n", ToggleKey);
        len += sprintf_s(buffer + len, bufferSize - len, "Enabled = %s\r\n", UseToggleKey ? "true" : "false");

        // Write to file
        DWORD written;
        WriteFile(file, buffer, len, &written, NULL);
        CloseHandle(file);
    }

    void LoadConfig()
    {
        HANDLE file = CreateFileA(
            "Config.toml", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (file == INVALID_HANDLE_VALUE)
        {
            SaveConfig(); // Create default config file
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

        char section[32] = { 0 };
        char* line = strtok(buffer, "\r\n");
        while (line)
        {
            while (*line == ' ' || *line == '\t')
                ++line;
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
            while (*key == ' ' || *key == '\t')
                ++key;
            char* keyEnd = key + strlen(key) - 1;
            while (keyEnd > key && (*keyEnd == ' ' || *keyEnd == '\t'))
                *keyEnd-- = '\0';

            while (*value == ' ' || *value == '\t')
                ++value;
            char* valueEnd = value + strlen(value) - 1;
            while (valueEnd > value && (*valueEnd == ' ' || *valueEnd == '\t'))
                *valueEnd-- = '\0';

            if (strcmp(section, "RecoilPresets") == 0)
            {
                if (strcmp(key, "Enabled") == 0)
                    EnableRC = (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
                else if (strcmp(key, "VerticalRecoil") == 0)
                    CurrentRecoil.Vertical = static_cast<float>(atof(value));
                else if (strcmp(key, "HorizontalRecoil") == 0)
                    CurrentRecoil.Horizontal = static_cast<float>(atof(value));
            }

            else if (strcmp(section, "Controller") == 0)
            {
                if (strcmp(key, "Enabled") == 0)
                    EnableController = (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
                else if (strcmp(key, "Multiplier") == 0)
                    ControllerMultiplier = atoi(value);
            }
            else if (strcmp(section, "ToggleKey") == 0)
            {
                if (strcmp(key, "ToggleKey") == 0)
                    ToggleKey = atoi(value);
                else if (strcmp(key, "Enabled") == 0)
                    UseToggleKey = (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
            }

            line = strtok(NULL, "\r\n");
        }

        delete[] buffer;
    }

} // namespace Files
