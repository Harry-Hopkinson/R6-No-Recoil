#include "../Globals.h"

#include "../core/String.h"
#include "../files/Files.h"
#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"

#include "../ui/Bitmap.h"
#include "../ui/widgets/Font.h"

#include "../utils/LayoutUtils.h"

#include <cstdio>

namespace Drawing
{

    void DrawWeaponDisplay(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
        const char* weaponStr = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex]
                                               : DefenderWeapons[SelectedOperatorIndex];

        // Titles
        Font::DrawCenteredText(memDC, operatorName, 0, 220, right, Font::GetLargeFont());
        Font::DrawCenteredText(memDC, "Select a weapon:", 0, 260, right, Font::GetMediumFont());

        // Parse weapon list
        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        SetStretchBltMode(memDC, HALFTONE);
        SetBrushOrgEx(memDC, 0, 0, nullptr);

        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX
                + i * (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH + LayoutUtils::WeaponDisplayLayout::WEAPON_SPACING);
            int y = startY;

            // Draw weapon image
            HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);
            Bitmap::DrawBitmap(
                memDC, weaponBmp, x, y, LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH,
                LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT, true);

            // Weapon name
            HFONT weaponFont = Font::GetLargeFont();
            HFONT oldFont = (HFONT)SelectObject(memDC, weaponFont);
            SetTextColor(memDC, RGB(0, 0, 0));
            RECT nameRect = { x, y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 15,
                              x + LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH,
                              y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 45 };
            DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(memDC, oldFont);

            const int buttonWidth = 200;
            const int buttonHeight = 45;
            const int buttonSpacing = 10;

            int buttonStartY = y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 60;
            int buttonStartX = x + (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH - buttonWidth) / 2;

            for (int p = 0; p < 3; ++p)
            {
                WeaponRecoil recoilData = Files::GetWeaponData(weapons[i], p + 1);

                RECT btnRect = { buttonStartX, buttonStartY + p * (buttonHeight + buttonSpacing), buttonStartX + buttonWidth,
                                 buttonStartY + p * (buttonHeight + buttonSpacing) + buttonHeight };

                // Uniform light gray fill
                HBRUSH fill = CreateSolidBrush(RGB(240, 240, 240));
                FillRect(memDC, &btnRect, fill);
                DeleteObject(fill);

                // Outline
                FrameRect(memDC, &btnRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

                char text[128];
                snprintf(text, sizeof(text), "Preset %d: V:%.1f, H:%.1f", p + 1, recoilData.Vertical, recoilData.Horizontal);

                HFONT btnFont = Font::GetMediumFont();
                HFONT oldBtnFont = (HFONT)SelectObject(memDC, btnFont);
                SetTextColor(memDC, RGB(0, 0, 0));
                DrawText(memDC, text, -1, &btnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(memDC, oldBtnFont);
            }
        }

        String::FreeWeaponList(weapons, weaponCount);

        // --- Note text ---
        const int sectionTop = bottom - LayoutUtils::WeaponDisplayLayout::SECTION_OFFSET_FROM_BOTTOM;
        const char* noteText = "Click a weapon or preset to select recoil settings.";
        Font::DrawCenteredText(memDC, noteText, 0, sectionTop + 260, right, Font::GetMediumFont());

        // --- Back button ---
        RECT backBtn = { 30, bottom - 80, 130, bottom - 31 };
        DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
    }

} // namespace Drawing
