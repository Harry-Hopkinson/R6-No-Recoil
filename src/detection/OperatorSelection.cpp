#include "ClickDetection.h"

#include "../Globals.h"

#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"

#include "../ui/Bitmap.h"
#include "../ui/widgets/Button.h"

#include "../utils/LayoutUtils.h"
#include "../utils/WindowUtils.h"

#define ceilf(x) ((int)(x) + ((x) > (int)(x) ? 1 : 0))
#define round(x) (ceilf((x) * 2.0f) / 2.0f)

namespace ButtonHandlers
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
        system("start https://discord.gg/H98vCAWQ3m");
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

    void HandleSaveConfig(HWND hwnd)
    {
        Files::SaveConfig();
        Files::SaveWeaponData(PresetIndex);

        WindowUtils::InvalidateWindow(hwnd);
    }

} // namespace ButtonHandlers

namespace ClickDetection
{

    static bool HandleButtonClick(HWND hwnd, int mouseX, int mouseY)
    {
        for (const auto& btn : Buttons::GetButtons())
        {
            if (btn.Contains(mouseX, mouseY))
            {
                switch (btn.id)
                {
                    case 1:
                        ButtonHandlers::HandleToggleRecoil(hwnd);
                        break;
                    case 2:
                        ButtonHandlers::HandleToggleKey(hwnd);
                        break;
                    case 3:
                        ButtonHandlers::HandleSwitchToAttackers(hwnd);
                        break;
                    case 4:
                        ButtonHandlers::HandleSwitchToDefenders(hwnd);
                        break;
                    case 5:
                        ButtonHandlers::HandleSupportProject(hwnd);
                        break;
                    case 6:
                        ButtonHandlers::HandleJoinDiscord(hwnd);
                        break;
                    case 7:
                        ButtonHandlers::HandleVerticalRecoilPlus(hwnd);
                        break;
                    case 8:
                        ButtonHandlers::HandleVerticalRecoilMinus(hwnd);
                        break;
                    case 9:
                        ButtonHandlers::HandleHorizontalRecoilPlus(hwnd);
                        break;
                    case 10:
                        ButtonHandlers::HandleHorizontalRecoilMinus(hwnd);
                        break;
                    case 11:
                        ButtonHandlers::HandleSaveConfig(hwnd);
                        break;
                }

                WindowUtils::InvalidateWindowNoChildren(hwnd);
                return true;
            }
        }
        return false;
    }

    void OperatorSelection(HWND hwnd, const int mouseX, const int mouseY)
    {
        // Check button clicks first
        if (HandleButtonClick(hwnd, mouseX, mouseY)) return;

        // Check operator grid clicks
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            RECT cellRect = LayoutUtils::OperatorGridLayout::GetCellRect(i);
            if (LayoutUtils::IsPointInRect(cellRect, mouseX, mouseY))
            {
                SelectedOperatorIndex = static_cast<int>(i);
                Scenes::ChangeCurrentScene(SceneType::WeaponDisplay);
                WindowUtils::InvalidateWindowNoChildren(hwnd);
                break;
            }
        }
    }

} // namespace ClickDetection
