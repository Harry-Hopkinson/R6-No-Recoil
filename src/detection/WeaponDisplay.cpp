#include "ClickDetection.h"

#include <windows.h>

#include "../core/String.h"
#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/widgets/Button.h"

namespace ClickDetection
{

    void ResetVariables()
    {
        SelectedWeaponIndex = -1;
        SelectedScopeType = ScopeType::NONE;
        SelectedGripType = GripType::NONE;
    }

    void WeaponDisplay(HWND hwnd, int right, int bottom, int mouseX, int mouseY)
    {
        const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                               : DefenderPrimaryWeapons[SelectedOperatorIndex];

        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        constexpr int imgWidth = 400;
        constexpr int imgHeight = 150;
        constexpr int spacing = 60;
        constexpr int btnWidth = 250;
        constexpr int gap = 60;
        constexpr int gripBtnWidth = 200;
        constexpr int gripGap = 40;

        int totalWidth = weaponCount * imgWidth + (weaponCount - 1) * spacing;
        int startX = (right - totalWidth) / 2;
        int startY = 80 + (bottom - 120 - (imgHeight + 50)) / 2;

        int sectionTop = bottom - 330;
        int centerX = right / 2;

        RECT magRect = { centerX - btnWidth - gap / 2, sectionTop + 60, centerX - gap / 2, sectionTop + 110 };
        RECT nonMagRect = { centerX + gap / 2, sectionTop + 60, centerX + btnWidth + gap / 2, sectionTop + 110 };

        RECT gripRects[3];
        gripRects[0] = { centerX - gripBtnWidth * 3 / 2 - gripGap, sectionTop + 180, centerX - gripBtnWidth / 2 - gripGap,
                         sectionTop + 230 }; // Horizontal
        gripRects[1] = { centerX - gripBtnWidth / 2, sectionTop + 180, centerX + gripBtnWidth / 2,
                         sectionTop + 230 }; // Vertical
        gripRects[2] = { centerX + gripBtnWidth / 2 + gripGap, sectionTop + 180, centerX + gripBtnWidth * 3 / 2 + gripGap,
                         sectionTop + 230 }; // Angled

        auto ProceedIfReady = [&](int selectedWeaponIndex) {
            if (selectedWeaponIndex == -1 || SelectedScopeType == ScopeType::NONE || SelectedGripType == GripType::NONE)
                return;

            SetRecoilModeFromWeapon(weapons[selectedWeaponIndex]);
            Files::SaveConfig();

            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);

            LastScopeType = SelectedScopeType;
            LastGripType = SelectedGripType;
            ResetVariables();

            if (CurrentWeapon)
                free((void*)CurrentWeapon);

            CurrentWeapon = _strdup(weapons[selectedWeaponIndex]);
        };


        // Scope buttons
        if (mouseY >= magRect.top && mouseY <= magRect.bottom)
        {
            if (mouseX >= magRect.left && mouseX <= magRect.right)
                SelectedScopeType = ScopeType::MAGNIFIED;
            else if (mouseX >= nonMagRect.left && mouseX <= nonMagRect.right)
                SelectedScopeType = ScopeType::NON_MAGNIFIED;

            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        // Grip buttons
        if (mouseY >= gripRects[0].top && mouseY <= gripRects[0].bottom)
        {
            if (mouseX >= gripRects[0].left && mouseX <= gripRects[0].right)
                SelectedGripType = GripType::HORIZONTAL;
            else if (mouseX >= gripRects[1].left && mouseX <= gripRects[1].right)
                SelectedGripType = GripType::VERTICAL;
            else if (mouseX >= gripRects[2].left && mouseX <= gripRects[2].right)
                SelectedGripType = GripType::ANGLED;

            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        // Weapon click detection
        for (int i = 0; i < weaponCount; ++i)
        {
            RECT clickRect = { startX + i * (imgWidth + spacing), startY, startX + i * (imgWidth + spacing) + imgWidth,
                               startY + imgHeight + 45 };

            if (mouseX >= clickRect.left && mouseX <= clickRect.right && mouseY >= clickRect.top && mouseY <= clickRect.bottom)
            {
                SelectedWeaponIndex = i;
                ProceedIfReady(i);
                InvalidateRect(hwnd, nullptr, TRUE);
                String::FreeWeaponList(weapons, weaponCount);
                return;
            }
        }

        // Back button
        RECT backBtn = { 30, bottom - 80, 130, bottom - 30 };
        if (mouseX >= backBtn.left && mouseX <= backBtn.right && mouseY >= backBtn.top && mouseY <= backBtn.bottom)
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);
            ResetVariables();
            InvalidateRect(hwnd, nullptr, TRUE);
        }

        String::FreeWeaponList(weapons, weaponCount);
    }

} // namespace ClickDetection
