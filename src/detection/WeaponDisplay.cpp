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

        int sectionTop = bottom - 250;
        int btnWidth = 250;
        int btnHeight = 50;
        int gap = 60;
        int centerX = right / 2;

        int nonMagLeft = centerX - btnWidth - gap / 2;
        int nonMagRight = centerX - gap / 2;
        int magLeft = centerX + gap / 2;
        int magRight = centerX + btnWidth + gap / 2;
        int btnTop = sectionTop + 60;
        int btnBottom = btnTop + btnHeight;

        if (mouseY >= btnTop && mouseY <= btnBottom)
        {
            if (mouseX >= nonMagLeft && mouseX <= nonMagRight)
                SelectedScopeType = ScopeType::MAGNIFIED;
            else if (mouseX >= magLeft && mouseX <= magRight)
                SelectedScopeType = ScopeType::NON_MAGNIFIED;

            if (SelectedWeaponIndex != -1)
            {
                SetRecoilModeFromWeapon(weapons[SelectedWeaponIndex]);
                Files::SaveConfig();

                Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
                Buttons::CreateOperatorSelectionButtons(hwnd);

                SelectedWeaponIndex = -1;
                SelectedScopeType = ScopeType::NONE;
            }

            InvalidateRect(hwnd, NULL, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (imgWidth + spacing);
            int y = startY;
            RECT clickRect = { x, y, x + imgWidth, y + imgHeight + 45 };

            if (mouseX >= clickRect.left && mouseX <= clickRect.right &&
                mouseY >= clickRect.top && mouseY <= clickRect.bottom)
            {
                SelectedWeaponIndex = i;

                if (SelectedScopeType != ScopeType::NONE)
                {
                    SetRecoilModeFromWeapon(weapons[i]);
                    Files::SaveConfig();

                    Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
                    Buttons::CreateOperatorSelectionButtons(hwnd);

                    SelectedWeaponIndex = -1;
                    SelectedScopeType = ScopeType::NONE;
                }

                InvalidateRect(hwnd, NULL, TRUE);
                String::FreeWeaponList(weapons, weaponCount);
                return;
            }
        }

        if (mouseX >= 30 && mouseX <= 130 && mouseY >= bottom - 80 && mouseY <= bottom - 30)
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);

            SelectedWeaponIndex = -1;
            SelectedScopeType = ScopeType::NONE;

            InvalidateRect(hwnd, NULL, TRUE);
        }

        String::FreeWeaponList(weapons, weaponCount);
    }

} // namespace ClickDetection
