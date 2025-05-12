#pragma once
#include <string>
#include "utils/clamp.hpp"

#include "RecoilPresets.hpp"
#include "../ui/UI.hpp"

void SaveConfig()
{
    HANDLE file = CreateFileA("Config.toml", GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) return;

    char buffer[512];
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

    len += wsprintfA(buffer + len, "# Toggle Key (e.g. 20 = CAPS LOCK)\r\n");
    len += wsprintfA(buffer + len, "ToggleKey = %d\r\n\r\n", ToggleKey);

    len += wsprintfA(buffer + len, "[UI]\r\n");
    len += wsprintfA(buffer + len, "DarkTheme = %s\r\n", DarkTheme ? "true" : "false");

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

    // Load as usual
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

    std::string section;
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
            if (end) section = std::string(line + 1, end);
            line = strtok(NULL, "\r\n");
            continue;
        }

        char* equal = strchr(line, '=');
        if (!equal)
        {
            line = strtok(NULL, "\r\n");
            continue;
        }

        std::string key(line, equal);
        std::string value(equal + 1);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (section == "RecoilPresets")
        {
            if (key == "Mode") SelectedMode = atoi(value.c_str());
            else if (key == "Enabled") EnableRC = (value == "true" || value == "1");
            else if (key == "LowSensVertical") RecoilPresets[0].Vertical = atoi(value.c_str());
            else if (key == "LowSensHorizontal") RecoilPresets[0].Horizontal = atoi(value.c_str());
            else if (key == "MediumSensVertical") RecoilPresets[1].Vertical = atoi(value.c_str());
            else if (key == "MediumSensHorizontal") RecoilPresets[1].Horizontal = atoi(value.c_str());
            else if (key == "HighSensVertical") RecoilPresets[2].Vertical = atoi(value.c_str());
            else if (key == "HighSensHorizontal") RecoilPresets[2].Horizontal = atoi(value.c_str());
            else if (key == "UltraSensVertical") RecoilPresets[3].Vertical = atoi(value.c_str());
            else if (key == "UltraSensHorizontal") RecoilPresets[3].Horizontal = atoi(value.c_str());
            else if (key == "ToggleKey") ToggleKey = atoi(value.c_str());
        }
        else if (section == "UI")
        {
            if (key == "DarkTheme") DarkTheme = (value == "true" || value == "1");
        }

        line = strtok(NULL, "\r\n");
    }

    delete[] buffer;

    SelectedMode = clamp(SelectedMode, 0, 3);
    CurrentRecoil = RecoilPresets[SelectedMode];
}
