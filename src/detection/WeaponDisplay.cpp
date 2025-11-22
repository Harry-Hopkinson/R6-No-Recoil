#include "ClickDetection.h"

#include "../Globals.h"

#include "../core/Keys.h"
#include "../core/String.h"
#include "../files/Files.h"

#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/widgets/Button.h"
#include "../utils/LayoutUtils.h"
#include "../utils/WindowUtils.h"

namespace ClickDetection
{

    void WeaponDisplay(HWND hwnd, int right, int bottom, int mouseX, int mouseY)
    {
        const char* weaponStr = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex]
                                               : DefenderWeapons[SelectedOperatorIndex];

        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons);

        struct WeaponListGuard
        {
            const char** list;
            int count;

            ~WeaponListGuard()
            {
                String::FreeWeaponList(list, count);
            }
        } guard{ weapons, weaponCount };

        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        auto ProceedIfReady = [&](int selectedWeaponIndex, int presetIndex) {
            PresetIndex = presetIndex;

            if (CurrentWeapon) free((void*)CurrentWeapon);

            CurrentWeapon = _strdup(weapons[selectedWeaponIndex]);

            CurrentRecoil = Files::GetWeaponData(CurrentWeapon, presetIndex);
            Files::SaveConfig();

            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons();
        };

        // Detect weapon and preset clicks
        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX
                + i * (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH + LayoutUtils::WeaponDisplayLayout::WEAPON_SPACING);
            int y = startY;

            // Check if weapon image clicked and default to Preset 1
            RECT weaponRect = LayoutUtils::WeaponDisplayLayout::GetWeaponRect(i, startX, startY);
            if (LayoutUtils::IsPointInRect(weaponRect, mouseX, mouseY))
            {
                ProceedIfReady(i, 1);
                WindowUtils::InvalidateWindowNoChildren(hwnd);

                return;
            }

            // Preset button rectangles
            const int buttonWidth = 200;
            const int buttonHeight = 45;
            const int buttonSpacing = 10;

            int buttonStartY = y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 60;
            int buttonStartX = x + (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH - buttonWidth) / 2;

            // Keybind toggle button
            const int keyWidth = 100;
            const int keyHeight = 30;

            const int keyStartX = x + (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH - keyWidth) / 2;
            const int keyStartY = y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 225;

            RECT keyRect = { keyStartX, keyStartY + keyHeight + 20, keyStartX + keyWidth,
                             keyStartY + keyHeight + 20 + keyHeight };

            if (LayoutUtils::IsPointInRect(keyRect, mouseX, mouseY))
            {
                switch (i)
                {
                    case 0:
                        PrimaryKeyEnabled = !PrimaryKeyEnabled;
                        break;
                    case 1:
                        SecondaryKeyEnabled = !SecondaryKeyEnabled;
                        break;
                    case 2:
                        TertiaryKeyEnabled = !TertiaryKeyEnabled;
                        break;
                }

                InvalidateRect(hwnd, nullptr, FALSE);
                return;
            }

            // Preset 1/2/3 buttons
            for (int p = 0; p < 3; ++p)
            {
                RECT btnRect = { buttonStartX, buttonStartY + p * (buttonHeight + buttonSpacing), buttonStartX + buttonWidth,
                                 buttonStartY + p * (buttonHeight + buttonSpacing) + buttonHeight };

                if (LayoutUtils::IsPointInRect(btnRect, mouseX, mouseY))
                {
                    ProceedIfReady(i, p + 1);
                    WindowUtils::InvalidateWindowNoChildren(hwnd);

                    return;
                }
            }
        }

        // Back button
        RECT backBtn = { 30, bottom - 80, 130, bottom - 30 };
        if (LayoutUtils::IsPointInRect(backBtn, mouseX, mouseY))
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons();
            WindowUtils::InvalidateWindowNoChildren(hwnd);

            return;
        }
    }

} // namespace ClickDetection
