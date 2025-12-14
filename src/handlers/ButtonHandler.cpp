#include "ButtonHandler.h"

#include <windows.h>

#include "../Globals.h"

#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"

#include "../ui/widgets/Button.h"

#include "../utils/WindowUtils.h"

#define ceilf(x) ((int)(x) + ((x) > (int)(x) ? 1 : 0))
#define round(x) (ceilf((x) * 2.0f) / 2.0f)

namespace ButtonHandler
{

    void HandleToggleRecoil(HWND hwnd)
    {
        EnableRC = !EnableRC;
        Files::SaveConfig();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleToggleKey(HWND hwnd)
    {
        UseToggleKey = !UseToggleKey;
        Files::SaveConfig();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleSwitchToAttackers(HWND hwnd)
    {
        IsAttackerView = true;
        Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
        Buttons::CreateOperatorSelectionButtons();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleSwitchToDefenders(HWND hwnd)
    {
        IsAttackerView = false;
        Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
        Buttons::CreateOperatorSelectionButtons();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleSupportProject(HWND)
    {
        system("start https://ko-fi.com/harryhopkinson");
    }

    void HandleJoinDiscord(HWND)
    {
        system("start https://discord.gg/TCMP8u8bKE");
    }

    void HandleVerticalRecoilPlus(HWND hwnd)
    {
        CurrentRecoil.Vertical = round(CurrentRecoil.Vertical + 0.5f);
        Files::SaveConfig();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleVerticalRecoilMinus(HWND hwnd)
    {
        CurrentRecoil.Vertical = round(CurrentRecoil.Vertical - 0.5f);
        Files::SaveConfig();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleHorizontalRecoilPlus(HWND hwnd)
    {
        CurrentRecoil.Horizontal = round(CurrentRecoil.Horizontal + 0.5f);
        Files::SaveConfig();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleHorizontalRecoilMinus(HWND hwnd)
    {
        CurrentRecoil.Horizontal = round(CurrentRecoil.Horizontal - 0.5f);
        Files::SaveConfig();

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleSavePreset(HWND hwnd)
    {
        Files::SaveWeaponData(PresetIndex);

        WindowUtils::InvalidateWindow(hwnd);
    }

    void HandleUpdateTool(HWND hwnd)
    {
        system("start UpdateTool.bat");

        WindowUtils::InvalidateWindow(hwnd);
    }

} // namespace ButtonHandler
