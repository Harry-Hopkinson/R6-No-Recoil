#include "ClickDetection.h"

#include <windows.h>

#include "../core/String.h"
#include "../files/Files.h"
#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/widgets/Button.h"

namespace ClickDetection
{

    void AttachmentDisplay(HWND hwnd, int right, int bottom,
                           int mouseX, int mouseY)
    {

        // Detect when clicking on an attachment
        // if (mouseX >= 150 && mouseX <= 250 && mouseY >= bottom - 120 && mouseY <= bottom - 70)
        // {
        //     Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
        //     Buttons::CreateOperatorSelectionButtons(hwnd);
        //     InvalidateRect(hwnd, NULL, TRUE);
        // }

        // Back button detection
        if (mouseX >= 30 && mouseX <= 130 && mouseY >= bottom - 80 && mouseY <= bottom - 30)
        {
            Scenes::ChangeCurrentScene(SceneType::WeaponDisplay);
            InvalidateRect(hwnd, NULL, TRUE);
        }
    }

} // namespace ClickDetection
