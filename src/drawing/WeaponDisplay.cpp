#include "../Globals.h"

#include "../core/String.h"
#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"

#include "../ui/Bitmap.h"
#include "../ui/widgets/Font.h"

#include "../utils/GdiHelpers.h"
#include "../utils/LayoutUtils.h"

namespace Drawing
{

    void DrawWeaponDisplay(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
        const char* weaponStr = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex]
                                               : DefenderWeapons[SelectedOperatorIndex];

        // Draw operator name and instruction text with correct fonts
        Font::DrawCenteredText(memDC, operatorName, 0, 220, right, Font::GetLargeFont());
        Font::DrawCenteredText(memDC, "Select a weapon:", 0, 260, right, Font::GetMediumFont());

        // Parse weapons
        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        SetStretchBltMode(memDC, HALFTONE);
        SetBrushOrgEx(memDC, 0, 0, nullptr);

        // Draw each weapon
        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH +
                                  LayoutUtils::WeaponDisplayLayout::WEAPON_SPACING);
            int y = startY;

            HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);
            Bitmap::DrawBitmap(memDC, weaponBmp, x, y,
                               LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH,
                               LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT, true);

            // Draw weapon name
            HFONT weaponFont = Font::GetLargeFont();
            HFONT oldFont = (HFONT)SelectObject(memDC, weaponFont);
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(0, 0, 0));

            RECT nameRect = { x, y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 15,
                              x + LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH,
                              y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 45 };
            DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(memDC, oldFont);

            // Underline selected weapon
            if (i == SelectedWeaponIndex)
            {
                int underlineY = y + LayoutUtils::WeaponDisplayLayout::WEAPON_HEIGHT + 48;
                int underlineLeft = x + LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH / 4;
                int underlineRight = x + (LayoutUtils::WeaponDisplayLayout::WEAPON_WIDTH * 3) / 4;

                GdiHelpers::ScopedPen pen(PS_SOLID, 4, RGB(50, 150, 255));
                GdiHelpers::ScopedSelectObject select(memDC, pen);
                MoveToEx(memDC, underlineLeft, underlineY, nullptr);
                LineTo(memDC, underlineRight, underlineY);
            }
        }

        String::FreeWeaponList(weapons, weaponCount);

        const int sectionTop = bottom - LayoutUtils::WeaponDisplayLayout::SECTION_OFFSET_FROM_BOTTOM;
        const char* noteText = "Note that clicking on the weapon will use Preset 1.";
        Font::DrawCenteredText(memDC, noteText, 0, sectionTop + 260, right, Font::GetMediumFont());

        // Back button
        RECT backBtn = { 30, bottom - 80, 130, bottom - 31 };
        DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
    }

} // namespace Drawing
