#include "Threads.h"

#include <windows.h>
#include <thread>

#include "../Globals.h"
#include "../core/Keys.h"
#include "../files/Files.h"
#include "../scenes/Scenes.h"
#include "../recoil/Recoil.h"
#include "../utils/StringUtils.h"

inline constexpr const char* WINDOW_TITLE = "R6 No Recoil";

inline constexpr int TOGGLE_DELAY_MS = 300;
inline constexpr int POLL_INTERVAL_MS = 50;

static HWND GetWindowHandle()
{
    static HWND hwnd = nullptr;
    if (!hwnd || !IsWindow(hwnd))
        hwnd = FindWindow(NULL, WINDOW_TITLE);
    return hwnd;
}

static void LoadWeaponRecoil(int weaponIndex)
{
    const char* weapons = IsAttackerView
        ? AttackerWeapons[SelectedOperatorIndex]
        : DefenderWeapons[SelectedOperatorIndex];

    char weaponName[16] = {};
    StringUtils::GetWeaponAtIndex(
        weapons, weaponIndex, weaponName, sizeof(weaponName));

    CurrentRecoil = Files::GetWeaponData(weaponName, 1);
    Files::SaveConfig();

    if (HWND hwnd = GetWindowHandle())
        InvalidateRect(hwnd, NULL, FALSE);
    std::this_thread::sleep_for(std::chrono::milliseconds(TOGGLE_DELAY_MS));
}

namespace Threads
{

    void ToggleRecoil()
    {
        while (Running)
        {
            if (UseToggleKey && (GetAsyncKeyState(ToggleKey) & 0x8000))
            {
                EnableRC = !EnableRC;
                Files::SaveConfig();

                if (HWND hwnd = GetWindowHandle())
                    InvalidateRect(hwnd, NULL, FALSE);
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(TOGGLE_DELAY_MS));
            }
            else if (
                PrimaryKeyEnabled && (GetAsyncKeyState(PrimaryKey) & 0x8000))
            {
                LoadWeaponRecoil(0);
            }
            else if (
                SecondaryKeyEnabled
                && (GetAsyncKeyState(SecondaryKey) & 0x8000))
            {
                LoadWeaponRecoil(1);
            }
            else if (
                TertiaryKeyEnabled && (GetAsyncKeyState(TertiaryKey) & 0x8000))
            {
                LoadWeaponRecoil(2);
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(POLL_INTERVAL_MS));
        }
    }
} // namespace Threads
