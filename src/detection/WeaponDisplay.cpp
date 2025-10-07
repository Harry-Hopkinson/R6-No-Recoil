#include "ClickDetection.h"

#include <windows.h>

#include "../core/String.h"
#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/widgets/Button.h"

namespace ClickDetection
{

    void WeaponDisplay(HWND hwnd, int right, int bottom, int mouseX, int mouseY)
    {
        const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                               : DefenderPrimaryWeapons[SelectedOperatorIndex];

        const char* weapons[3] = { NULL, NULL, NULL };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        int imgWidth = 400;
        int imgHeight = 150;
        int spacing = 60;

        int totalWidth = weaponCount * imgWidth + (weaponCount - 1) * spacing;
        int startX = (right - totalWidth) / 2;

        int availableHeight = bottom - 120;
        int contentHeight = imgHeight + 50;
        int startY = 120 + (availableHeight - contentHeight) / 2;

        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (imgWidth + spacing);
            int y = startY;

            RECT clickRect = { x, y, x + imgWidth, y + imgHeight + 45 };

            if (mouseX >= clickRect.left && mouseX <= clickRect.right && mouseY >= clickRect.top && mouseY <= clickRect.bottom)
            {
                SetRecoilModeFromWeapon(weapons[i]);
                Files::SaveConfig();

                Scenes::ChangeCurrentScene(SceneType::AttachmentDisplay);
                InvalidateRect(hwnd, NULL, TRUE);

                String::FreeWeaponList(weapons, weaponCount);
            }
        }
        // Back button detection
        if (mouseX >= 30 && mouseX <= 130 && mouseY >= bottom - 80 && mouseY <= bottom - 30)
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
        }
    }

} // namespace ClickDetection
