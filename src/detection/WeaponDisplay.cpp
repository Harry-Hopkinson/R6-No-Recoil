#include "ClickDetection.h"

#include "../core/String.h"
#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/widgets/Button.h"
#include "../utils/LayoutUtils.h"

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
        const char* weaponStr = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex]
                                               : DefenderWeapons[SelectedOperatorIndex];

        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        // Calculate layout
        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        RECT magRect, nonMagRect;
        LayoutUtils::WeaponDisplayLayout::GetScopeButtonRects(right, bottom, magRect, nonMagRect);

        RECT horizontalRect, verticalRect, angledRect;
        LayoutUtils::WeaponDisplayLayout::GetGripButtonRects(right, bottom, horizontalRect, verticalRect, angledRect);

        auto ProceedIfReady = [&](int selectedWeaponIndex)
        {
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
        if (LayoutUtils::IsPointInRect(magRect, mouseX, mouseY))
        {
            SelectedScopeType = ScopeType::MAGNIFIED;
            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }
        else if (LayoutUtils::IsPointInRect(nonMagRect, mouseX, mouseY))
        {
            SelectedScopeType = ScopeType::NON_MAGNIFIED;
            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        // Grip buttons
        if (LayoutUtils::IsPointInRect(horizontalRect, mouseX, mouseY))
        {
            SelectedGripType = GripType::HORIZONTAL;
            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }
        else if (LayoutUtils::IsPointInRect(verticalRect, mouseX, mouseY))
        {
            SelectedGripType = GripType::VERTICAL;
            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }
        else if (LayoutUtils::IsPointInRect(angledRect, mouseX, mouseY))
        {
            SelectedGripType = GripType::ANGLED;
            ProceedIfReady(SelectedWeaponIndex);
            InvalidateRect(hwnd, nullptr, TRUE);
            String::FreeWeaponList(weapons, weaponCount);
            return;
        }

        // Weapon click detection
        for (int i = 0; i < weaponCount; ++i)
        {
            RECT weaponRect = LayoutUtils::WeaponDisplayLayout::GetWeaponRect(i, startX, startY);

            if (LayoutUtils::IsPointInRect(weaponRect, mouseX, mouseY))
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
        if (LayoutUtils::IsPointInRect(backBtn, mouseX, mouseY))
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);
            ResetVariables();
            InvalidateRect(hwnd, nullptr, TRUE);
        }

        String::FreeWeaponList(weapons, weaponCount);
    }

} // namespace ClickDetection
