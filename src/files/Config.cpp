#include "Files.h"

#include "../Globals.h"
#include "../core/Keys.h"
#include "../recoil/Recoil.h"

#include "../ui/Themes.h"

#include "../utils/FileUtils.h"
#include "../utils/StringUtils.h"

namespace Files
{

    void SaveConfig()
    {
        char buffer[600];
        int len = 0;
        const int bufferSize = sizeof(buffer);

        // [General]
        len += sprintf_s(buffer + len, bufferSize - len, "[General]\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len, "Enabled = %s\r\n",
            EnableRC ? "true" : "false");
        len += sprintf_s(
            buffer + len, bufferSize - len, "VerticalRecoil = %.2f\r\n",
            CurrentRecoil.Vertical);
        len += sprintf_s(
            buffer + len, bufferSize - len, "HorizontalRecoil = %.2f\r\n",
            CurrentRecoil.Horizontal);
        len += sprintf_s(
            buffer + len, bufferSize - len, "RapidFire = %s\r\n",
            RapidFire ? "true" : "false");
        len += sprintf_s(buffer + len, bufferSize - len, "\r\n");

        // [Controller]
        len += sprintf_s(buffer + len, bufferSize - len, "[Controller]\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len, "Enabled = %s\r\n",
            EnableController ? "true" : "false");
        len += sprintf_s(
            buffer + len, bufferSize - len, "Multiplier = %d\r\n",
            ControllerMultiplier);
        len += sprintf_s(buffer + len, bufferSize - len, "\r\n");

        // [Keys]
        len += sprintf_s(buffer + len, bufferSize - len, "[Keys]\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len,
            "# Use "
            "https://learn.microsoft.com/en-us/windows/win32/inputdev/"
            "virtual-key-codes\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len,
            "# And then convert the key code to decimal\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len,
            "# Toggle Key (e.g. 20 = CAPS LOCK)\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len, "ToggleKey = %d\r\n",
            ToggleKey);
        len += sprintf_s(
            buffer + len, bufferSize - len, "ToggleKeyEnabled = %s\r\n",
            UseToggleKey ? "true" : "false");
        len += sprintf_s(
            buffer + len, bufferSize - len, "PrimaryKey = %d\r\n", PrimaryKey);
        len += sprintf_s(
            buffer + len, bufferSize - len, "SecondaryKey = %d\r\n",
            SecondaryKey);
        len += sprintf_s(
            buffer + len, bufferSize - len, "TertiaryKey = %d\r\n",
            TertiaryKey);
        len += sprintf_s(buffer + len, bufferSize - len, "\r\n");

        len += sprintf_s(buffer + len, bufferSize - len, "[Theme]\r\n");
        len += sprintf_s(
            buffer + len, bufferSize - len,
            "BackgroundColour = RGB(%d, %d, %d)\r\n",
            GetRValue(BackgroundColour), GetGValue(BackgroundColour),
            GetBValue(BackgroundColour));
        len += sprintf_s(
            buffer + len, bufferSize - len, "TextColour = RGB(%d, %d, %d)\r\n",
            GetRValue(TextColour), GetGValue(TextColour),
            GetBValue(TextColour));
        len += sprintf_s(
            buffer + len, bufferSize - len,
            "ButtonColour = RGB(%d, %d, %d)\r\n", GetRValue(ButtonColour),
            GetGValue(ButtonColour), GetBValue(ButtonColour));
        len += sprintf_s(
            buffer + len, bufferSize - len, "LineColour = RGB(%d, %d, %d)\r\n",
            GetRValue(LineColour), GetGValue(LineColour),
            GetBValue(LineColour));

        FileUtils::WriteFileFromMemory("Config.toml", buffer, len);
    }

    void LoadConfig()
    {
        char* buffer = FileUtils::ReadFileToMemory("Config.toml");

        if (!buffer)
        {
            SaveConfig(); // Create default config file
            return;
        }

        char section[32] = { 0 };
        char* line = strtok(buffer, "\r\n");
        while (line)
        {
            line = StringUtils::TrimWhitespace(line);

            if (*line == '\0' || *line == '#')
            {
                line = strtok(NULL, "\r\n");
                continue;
            }

            if (line[0] == '[')
            {
                char* end = strchr(line, ']');
                if (end
                    && static_cast<size_t>(end - line - 1) < sizeof(section))
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
            char* key = StringUtils::TrimWhitespace(line);
            char* value = StringUtils::TrimWhitespace(equal + 1);

            if (StringUtils::StringEquals(section, "General"))
            {
                if (StringUtils::StringEquals(key, "Enabled"))
                    EnableRC
                        = (StringUtils::StringEquals(value, "true")
                           || StringUtils::StringEquals(value, "1"));
                else if (StringUtils::StringEquals(key, "VerticalRecoil"))
                    CurrentRecoil.Vertical = static_cast<float>(atof(value));
                else if (StringUtils::StringEquals(key, "HorizontalRecoil"))
                    CurrentRecoil.Horizontal = static_cast<float>(atof(value));
                else if (StringUtils::StringEquals(key, "RapidFire"))
                    RapidFire
                        = (StringUtils::StringEquals(value, "true")
                           || StringUtils::StringEquals(value, "1"));
            }
            else if (StringUtils::StringEquals(section, "Controller"))
            {
                if (StringUtils::StringEquals(key, "Enabled"))
                    EnableController
                        = (StringUtils::StringEquals(value, "true")
                           || StringUtils::StringEquals(value, "1"));
                else if (StringUtils::StringEquals(key, "Multiplier"))
                    ControllerMultiplier = atoi(value);
            }
            else if (StringUtils::StringEquals(section, "Keys"))
            {
                if (StringUtils::StringEquals(key, "ToggleKey"))
                    ToggleKey = atoi(value);
                else if (StringUtils::StringEquals(key, "ToggleKeyEnabled"))
                    UseToggleKey
                        = (StringUtils::StringEquals(value, "true")
                           || StringUtils::StringEquals(value, "1"));
                else if (StringUtils::StringEquals(key, "PrimaryKey"))
                    PrimaryKey = atoi(value);
                else if (StringUtils::StringEquals(key, "SecondaryKey"))
                    SecondaryKey = atoi(value);
                else if (StringUtils::StringEquals(key, "TertiaryKey"))
                    TertiaryKey = atoi(value);
            }
            else if (StringUtils::StringEquals(section, "Theme"))
            {
                if (StringUtils::StringEquals(key, "BackgroundColour"))
                {
                    int r, g, b;
                    if (sscanf_s(value, "RGB(%d, %d, %d)", &r, &g, &b) == 3)
                        BackgroundColour = RGB(r, g, b);
                }
                else if (StringUtils::StringEquals(key, "TextColour"))
                {
                    int r, g, b;
                    if (sscanf_s(value, "RGB(%d, %d, %d)", &r, &g, &b) == 3)
                        TextColour = RGB(r, g, b);
                }
                else if (StringUtils::StringEquals(key, "ButtonColour"))
                {
                    int r, g, b;
                    if (sscanf_s(value, "RGB(%d, %d, %d)", &r, &g, &b) == 3)
                        ButtonColour = RGB(r, g, b);
                }
                else if (StringUtils::StringEquals(key, "LineColour"))
                {
                    int r, g, b;
                    if (sscanf_s(value, "RGB(%d, %d, %d)", &r, &g, &b) == 3)
                        LineColour = RGB(r, g, b);
                }
            }

            line = strtok(NULL, "\r\n");
        }

        delete[] buffer;
    }

} // namespace Files
