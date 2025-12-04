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

    void WeaponDisplay(HWND hwnd, int windowWidth, int windowHeight, int mouseX, int mouseY)
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
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, windowWidth, windowHeight, startX, startY);

        // Get dimensions
        int weaponWidth = LayoutUtils::WeaponDisplayLayout::GetWeaponWidth(windowWidth);
        int weaponHeight = LayoutUtils::WeaponDisplayLayout::GetWeaponHeight(windowHeight);
        int spacing = LayoutUtils::WeaponDisplayLayout::GetWeaponSpacing(windowWidth);

        auto ProceedIfReady = [&](int selectedWeaponIndex, int presetIndex) {
            PresetIndex = presetIndex;

            if (CurrentWeapon)
                free((void*)CurrentWeapon);

            CurrentWeapon = _strdup(weapons[selectedWeaponIndex]);

            CurrentRecoil = Files::GetWeaponData(CurrentWeapon, presetIndex);
            Files::SaveConfig();

            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons();
        };

        // Detect weapon and preset clicks
        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (weaponWidth + spacing);
            int y = startY;

            // Check if weapon image clicked and default to Preset 1
            RECT weaponRect = LayoutUtils::WeaponDisplayLayout::GetWeaponRect(i, windowWidth, windowHeight, startX, startY);
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

            int buttonStartY = y + weaponHeight + 60;
            int buttonStartX = x + (weaponWidth - buttonWidth) / 2;

            // Keybind toggle button
            const int keyWidth = 100;
            const int keyHeight = 30;

            const int keyStartX = x + (weaponWidth - keyWidth) / 2;
            const int keyStartY = y + weaponHeight + 225;

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
        RECT backBtn = { 30, windowHeight - 80, 130, windowHeight - 30 };
        if (LayoutUtils::IsPointInRect(backBtn, mouseX, mouseY))
        {
            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons();
            WindowUtils::InvalidateWindowNoChildren(hwnd);

            return;
        }
    }

} // namespace ClickDetection
