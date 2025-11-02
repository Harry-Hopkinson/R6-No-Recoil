#include "Threads.h"

#include <windows.h>
#include <thread>

#include "../Globals.h"
#include "../core/Keys.h"
#include "../files/Files.h"
#include "../scenes/Scenes.h"
#include "../recoil/Recoil.h"

constexpr const char* WINDOW_TITLE = "R6 No Recoil";

constexpr int TOGGLE_DELAY_MS = 300;
constexpr int POLL_INTERVAL_MS = 50;

void GetWeaponAtIndex(const char* weapons, int index, char* out, size_t out_size)
{
    if (!weapons || !out || out_size == 0)
        return;

    int current = 0;
    const char* start = weapons;

    while (*start)
    {
        while (*start && isspace((unsigned char)*start))
            ++start;

        const char* end = start;
        while (*end && *end != ',')
            ++end;

        if (current == index)
        {
            const char* trim_end = end;
            while (trim_end > start && isspace((unsigned char)*(trim_end - 1)))
                --trim_end;

            size_t len = trim_end - start;
            if (len >= out_size)
                len = out_size - 1;

            memcpy(out, start, len);
            out[len] = '\0';
            return;
        }

        if (*end == ',')
            ++end;

        start = end;
        ++current;
    }

    out[0] = '\0';
}

namespace Threads
{

    static HWND GetWindowHandle()
    {
        static HWND hwnd = nullptr;
        if (!hwnd || !IsWindow(hwnd))
            hwnd = FindWindow(NULL, WINDOW_TITLE);
        return hwnd;
    }

    static void LoadWeaponRecoil(int weaponIndex)
    {
        const char* weapons = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex] : DefenderWeapons[SelectedOperatorIndex];

        char weaponName[16] = {};
        GetWeaponAtIndex(weapons, weaponIndex, weaponName, sizeof(weaponName));

        CurrentRecoil = Files::GetWeaponData(weaponName, 1);
        Files::SaveConfig();

        if (HWND hwnd = GetWindowHandle())
            InvalidateRect(hwnd, NULL, TRUE);
        std::this_thread::sleep_for(std::chrono::milliseconds(TOGGLE_DELAY_MS));
    }

    void ToggleRecoil()
    {
        while (Running)
        {
            if (UseToggleKey && (GetAsyncKeyState(ToggleKey) & 0x8000))
            {
                EnableRC = !EnableRC;
                Files::SaveConfig();

                if (HWND hwnd = GetWindowHandle())
                    InvalidateRect(hwnd, NULL, TRUE);
                std::this_thread::sleep_for(std::chrono::milliseconds(TOGGLE_DELAY_MS));
            }
            else if (PrimaryKeyEnabled && (GetAsyncKeyState(PrimaryKey) & 0x8000))
            {
                LoadWeaponRecoil(0);
            }
            else if (SecondaryKeyEnabled && (GetAsyncKeyState(SecondaryKey) & 0x8000))
            {
                LoadWeaponRecoil(1);
            }
            else if (TertiaryKeyEnabled && (GetAsyncKeyState(TertiaryKey) & 0x8000))
            {
                LoadWeaponRecoil(2);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(POLL_INTERVAL_MS));
        }
    }
} // namespace Threads
