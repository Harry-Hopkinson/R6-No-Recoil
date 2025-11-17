#include "ClickDetection.h"

#include "../scenes/Scenes.h"
#include "../ui/Bitmap.h"
#include "../ui/widgets/Button.h"
#include "../utils/LayoutUtils.h"
#include "../utils/WindowUtils.h"

namespace ClickDetection
{

    void OperatorSelection(HWND hwnd, const int mouseX, const int mouseY)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            RECT cellRect = LayoutUtils::OperatorGridLayout::GetCellRect(i);

            if (LayoutUtils::IsPointInRect(cellRect, mouseX, mouseY))
            {
                SelectedOperatorIndex = static_cast<int>(i);
                Scenes::ChangeCurrentScene(SceneType::WeaponDisplay);

                for (const auto& button : Buttons::GetButtons())
                    ShowWindow(button.GetHWND(), SW_HIDE);

                    WindowUtils::InvalidateWindowNoChildren(hwnd);
                break;
            }
        }
    }

} // namespace ClickDetection
