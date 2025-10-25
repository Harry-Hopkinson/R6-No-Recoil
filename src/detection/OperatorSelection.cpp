#include "ClickDetection.h"

#include <windows.h>

#include "../scenes/Scenes.h"
#include "../ui/Bitmap.h"
#include "../ui/widgets/Button.h"

namespace ClickDetection
{

    void OperatorSelection(HWND hwnd, const int mouseX, const int mouseY)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            int x = 30 + (i % 6) * (110 + 10);
            int y = 50 + (int)(i / 6) * (110 + 10);

            if (mouseX >= x && mouseX <= x + 110 && mouseY >= y && mouseY <= y + 110)
            {
                SelectedOperatorIndex = static_cast<int>(i);
                Scenes::ChangeCurrentScene(SceneType::WeaponDisplay);

                for (const auto& button : Buttons::GetButtons())
                    ShowWindow(button.GetHWND(), SW_HIDE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
    }

} // namespace ClickDetection
