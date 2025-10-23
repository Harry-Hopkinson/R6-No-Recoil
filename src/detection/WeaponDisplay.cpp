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
        int startY = 80 + (availableHeight - contentHeight) / 2;

        int sectionTop = bottom - 330;
        int btnWidth = 250;
        int btnHeight = 50;
        int gap = 60;
        int centerX = right / 2;

        // Scope buttons
        int magLeft = centerX - btnWidth - gap / 2;
        int magRight = centerX - gap / 2;
        int nonMagLeft = centerX + gap / 2;
        int nonMagRight = centerX + btnWidth + gap / 2;
        int btnTop = sectionTop + 60;
        int btnBottom = btnTop + btnHeight;

        // Grip buttons
        int gripTop = sectionTop + 130;
        int gripBtnWidth = 200;
        int gripBtnHeight = 50;
        int gripGap = 40;
        int gripBtnTop = gripTop + 50;
        int gripBtnBottom = gripBtnTop + gripBtnHeight;

        int horizontalLeft = centerX - gripBtnWidth - gripGap - gripBtnWidth / 2;
        int horizontalRight = centerX - gripGap - gripBtnWidth / 2;
        int verticalLeft = centerX - gripBtnWidth / 2;
        int verticalRight = centerX + gripBtnWidth / 2;
        int angledLeft = centerX + gripGap + gripBtnWidth / 2;
        int angledRight = centerX + gripBtnWidth + gripGap + gripBtnWidth / 2;

        auto ProceedIfReady = [&](int selectedWeaponIndex)
        {
            if (selectedWeaponIndex != -1 && SelectedScopeType != ScopeType::NONE && SelectedGripType != GripType::NONE)
            {
                SetRecoilModeFromWeapon(weapons[selectedWeaponIndex]);
                Files::SaveConfig();

                Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
                Buttons::CreateOperatorSelectionButtons(hwnd);

                SelectedWeaponIndex = -1;
                SelectedScopeType = ScopeType::NONE;
                SelectedGripType = GripType::NONE;
            }
        };

        // Scope click detection
        if (mouseY >= btnTop && mouseY <= btnBottom)
        {
            if (mouseX >= magLeft && mouseX <= magRight)
                SelectedScopeType = ScopeType::MAGNIFIED;
            else if (mouseX >= nonMagLeft && mouseX <= nonMagRight)
                SelectedScopeType = ScopeType::NON_MAGNIFIED;

            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, NULL, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        // Grip click detection
        if (mouseY >= gripBtnTop && mouseY <= gripBtnBottom)
        {
            if (mouseX >= horizontalLeft && mouseX <= horizontalRight)
                SelectedGripType = GripType::HORIZONTAL;
            else if (mouseX >= verticalLeft && mouseX <= verticalRight)
                SelectedGripType = GripType::VERTICAL;
            else if (mouseX >= angledLeft && mouseX <= angledRight)
                SelectedGripType = GripType::ANGLED;


            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, NULL, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        // Weapon click detection
        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (imgWidth + spacing);
            int y = startY;
            RECT clickRect = { x, y, x + imgWidth, y + imgHeight + 45 };

            if (mouseX >= clickRect.left && mouseX <= clickRect.right &&
                mouseY >= clickRect.top && mouseY <= clickRect.bottom)
            {
                SelectedWeaponIndex = i;

                ProceedIfReady(i);
                InvalidateRect(hwnd, NULL, TRUE);
                String::FreeWeaponList(weapons, weaponCount);
                return;
            }
        }

        // Back button
        if (mouseX >= 30 && mouseX <= 130 && mouseY >= bottom - 80 && mouseY <= bottom - 30)
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);

            SelectedWeaponIndex = -1;
            SelectedScopeType = ScopeType::NONE;
            SelectedGripType = GripType::NONE;

            InvalidateRect(hwnd, NULL, TRUE);
        }

        String::FreeWeaponList(weapons, weaponCount);
    }

} // namespace ClickDetection
