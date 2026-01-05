#include "ClickDetection.h"

#include "../handlers/ButtonHandler.h"

#include "../scenes/Scenes.h"

#include "../ui/Bitmap.h"
#include "../ui/widgets/Button.h"

#include "../utils/LayoutUtils.h"
#include "../utils/WindowUtils.h"

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
                        ButtonHandler::HandleToggleRecoil(hwnd);
                        break;
                    case 2:
                        ButtonHandler::HandleToggleKey(hwnd);
                        break;
                    case 3:
                        ButtonHandler::HandleSwitchToAttackers(hwnd);
                        break;
                    case 4:
                        ButtonHandler::HandleSwitchToDefenders(hwnd);
                        break;
                    case 5:
                        ButtonHandler::HandleSupportProject(hwnd);
                        break;
                    case 6:
                        ButtonHandler::HandleJoinDiscord(hwnd);
                        break;
                    case 7:
                        ButtonHandler::HandleVerticalRecoilPlus(hwnd);
                        break;
                    case 8:
                        ButtonHandler::HandleVerticalRecoilMinus(hwnd);
                        break;
                    case 9:
                        ButtonHandler::HandleHorizontalRecoilPlus(hwnd);
                        break;
                    case 10:
                        ButtonHandler::HandleHorizontalRecoilMinus(hwnd);
                        break;
                    case 11:
                        ButtonHandler::HandleSavePreset(hwnd);
                        break;
                    case 12:
                        ButtonHandler::HandleUpdateTool(hwnd);
                        break;

                    default:
                        break;
                }

                WindowUtils::InvalidateWindowNoChildren(hwnd);
                return true;
            }
        }
        return false;
    }

    void OperatorSelection(
        HWND hwnd, const int windowWidth, const int windowHeight, int mouseX,
        int mouseY)
    {
        // Check button clicks first
        if (HandleButtonClick(hwnd, mouseX, mouseY))
            return;

        // Check operator grid clicks
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            RECT cellRect = LayoutUtils::OperatorGridLayout::GetCellRect(
                i, windowWidth, windowHeight);
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
