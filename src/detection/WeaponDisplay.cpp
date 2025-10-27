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

        // Calculate layout
        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        auto ProceedIfReady = [&](int selectedWeaponIndex)
        {
            SetRecoilModeFromWeapon(weapons[selectedWeaponIndex]);
            Files::SaveConfig();

            Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
            Buttons::CreateOperatorSelectionButtons(hwnd);

            SelectedWeaponIndex = -1;

            if (CurrentWeapon)
                free((void*)CurrentWeapon);

            CurrentWeapon = _strdup(weapons[selectedWeaponIndex]);
        };

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
            SelectedWeaponIndex = -1;
            InvalidateRect(hwnd, nullptr, TRUE);
        }

        String::FreeWeaponList(weapons, weaponCount);
    }

} // namespace ClickDetection
