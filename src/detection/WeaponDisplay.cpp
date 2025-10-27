#include "ClickDetection.h"

#include "../core/String.h"
#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/widgets/Button.h"
#include "../utils/LayoutUtils.h"

namespace ClickDetection
{

    void WeaponDisplay(HWND hwnd, int right, int bottom, int mouseX, int mouseY)
    {
        const char* weaponStr = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex]
                                               : DefenderWeapons[SelectedOperatorIndex];

        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        auto ProceedIfReady = [&](int selectedWeaponIndex, int presetIndex)
        {
            PresetIndex = presetIndex;

            Files::SaveConfig();

            if (CurrentWeapon)
                free((void*)CurrentWeapon);
            CurrentWeapon = _strdup(weapons[selectedWeaponIndex]);

            CurrentRecoil = Files::GetWeaponData(CurrentWeapon, PresetIndex);

            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);
        };

        // Detect weapon and preset clicks
        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX
                + i * (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH + LayoutUtils::WeaponDisplayLayout::WEAPON_SPACING);
            int y = startY;

            // Weapon bitmap area
            RECT weaponRect = LayoutUtils::WeaponDisplayLayout::GetWeaponRect(i, startX, startY);
            if (LayoutUtils::IsPointInRect(weaponRect, mouseX, mouseY))
            {
                ProceedIfReady(i, 1); // default to Preset 1
                InvalidateRect(hwnd, nullptr, TRUE);
                String::FreeWeaponList(weapons, weaponCount);
                return;
            }

            // Vertical preset buttons
            const int buttonWidth = 200;
            const int buttonHeight = 45;
            const int buttonSpacing = 10;

            int buttonStartY = y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 60;
            int buttonStartX = x + (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH - buttonWidth) / 2;

            for (int p = 0; p < 3; ++p)
            {
                RECT btnRect = { buttonStartX, buttonStartY + p * (buttonHeight + buttonSpacing), buttonStartX + buttonWidth,
                                 buttonStartY + p * (buttonHeight + buttonSpacing) + buttonHeight };

                if (LayoutUtils::IsPointInRect(btnRect, mouseX, mouseY))
                {
                    ProceedIfReady(i, p + 1); // Preset 1/2/3
                    InvalidateRect(hwnd, nullptr, TRUE);
                    String::FreeWeaponList(weapons, weaponCount);
                    return;
                }
            }
        }

        // Back button detection
        RECT backBtn = { 30, bottom - 80, 130, bottom - 30 };
        if (LayoutUtils::IsPointInRect(backBtn, mouseX, mouseY))
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);
            InvalidateRect(hwnd, nullptr, TRUE);
        }

        String::FreeWeaponList(weapons, weaponCount);
    }

} // namespace ClickDetection
