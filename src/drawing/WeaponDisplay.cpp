#include "../Globals.h"
#include "../core/String.h"
#include "../recoil/Recoil.h"
#include "../scenes/Scenes.h"
#include "../ui/Bitmap.h"
#include "../ui/widgets/Font.h"

#include <windows.h>

namespace Drawing
{

    void DrawWeaponDisplay(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
        const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                               : DefenderPrimaryWeapons[SelectedOperatorIndex];

        Font::DrawCenteredText(memDC, operatorName, 0, 160, right, Font::GetLargeFont());
        Font::DrawCenteredText(memDC, "Select a primary weapon:", 0, 200, right, Font::GetMediumFont());

        // Parse available weapons
        const char* weapons[3] = { NULL, NULL, NULL };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        int imgWidth = 400;
        int imgHeight = 150;
        int totalWidth = weaponCount * imgWidth + (weaponCount - 1);
        int startX = (right - totalWidth) / 2;

        // Shift weapon images up
        int availableHeight = bottom - 120;
        int contentHeight = imgHeight + 50;
        int startY = 40 + (availableHeight - contentHeight) / 2;

        SetStretchBltMode(memDC, HALFTONE);
        SetBrushOrgEx(memDC, 0, 0, NULL);

        // Draw each weapon
        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * imgWidth;
            int y = startY;

            HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);
            Bitmap::DrawBitmap(memDC, weaponBmp, x, y, imgWidth, imgHeight, true);

            RECT nameRect = { x, y + imgHeight + 15, x + imgWidth, y + imgHeight + 45 };
            HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetLargeFont());
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(0, 0, 0));
            DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(memDC, oldFont);

            // Underline for selected weapon
            if (i == SelectedWeaponIndex)
            {
                int underlineY = y + imgHeight + 48;
                int underlineLeft = x + imgWidth * 0.25;
                int underlineRight = x + imgWidth * 0.75;

                HPEN pen = CreatePen(PS_SOLID, 4, RGB(50, 150, 255));
                HPEN oldPen = (HPEN)SelectObject(memDC, pen);

                MoveToEx(memDC, underlineLeft, underlineY, NULL);
                LineTo(memDC, underlineRight, underlineY);

                SelectObject(memDC, oldPen);
                DeleteObject(pen);
            }
        }

        String::FreeWeaponList(weapons, weaponCount);

        // Scope Section
        const int sectionTop = bottom - 330;

        Font::DrawCenteredText(memDC, "Scope", 0, sectionTop, right, Font::GetLargeFont());

        int btnWidth = 250;
        int btnHeight = 50;
        int gap = 60;
        int centerX = right / 2;

        RECT magBtn = { centerX - btnWidth - gap / 2, sectionTop + 60, centerX - gap / 2, sectionTop + 60 + btnHeight };
        RECT nonMagBtn = { centerX + gap / 2, sectionTop + 60, centerX + btnWidth + gap / 2, sectionTop + 60 + btnHeight };

        if (SelectedScopeType == ScopeType::MAGNIFIED)
            FillRect(memDC, &magBtn, CreateSolidBrush(RGB(200, 230, 255)));
        else if (SelectedScopeType == ScopeType::NON_MAGNIFIED)
            FillRect(memDC, &nonMagBtn, CreateSolidBrush(RGB(200, 230, 255)));

        FrameRect(memDC, &magBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawText(memDC, "Magnified", -1, &magBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        FrameRect(memDC, &nonMagBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawText(memDC, "Non-Magnifying", -1, &nonMagBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Grip Section
        const int gripTop = sectionTop + 130;

        Font::DrawCenteredText(memDC, "Grip", 0, gripTop, right, Font::GetLargeFont());

        int gripBtnWidth = 200;
        int gripBtnHeight = 50;
        int gripGap = 40;

        RECT horizontalBtn = { centerX - gripBtnWidth - gripGap - gripBtnWidth / 2, gripTop + 50,
                               centerX - gripGap - gripBtnWidth / 2, gripTop + 50 + gripBtnHeight };
        RECT verticalBtn = { centerX - gripBtnWidth / 2, gripTop + 50, centerX + gripBtnWidth / 2,
                             gripTop + 50 + gripBtnHeight };
        RECT angledBtn = { centerX + gripGap + gripBtnWidth / 2, gripTop + 50,
                           centerX + gripBtnWidth + gripGap + gripBtnWidth / 2, gripTop + 50 + gripBtnHeight };

        if (SelectedGripType == GripType::HORIZONTAL)
            FillRect(memDC, &horizontalBtn, CreateSolidBrush(RGB(200, 230, 255)));
        else if (SelectedGripType == GripType::VERTICAL)
            FillRect(memDC, &verticalBtn, CreateSolidBrush(RGB(200, 230, 255)));
        else if (SelectedGripType == GripType::ANGLED)
            FillRect(memDC, &angledBtn, CreateSolidBrush(RGB(200, 230, 255)));

        // Draw frames and labels
        FrameRect(memDC, &horizontalBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawText(memDC, "Horizontal", -1, &horizontalBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        FrameRect(memDC, &verticalBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawText(memDC, "Vertical", -1, &verticalBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        FrameRect(memDC, &angledBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawText(memDC, "Angled", -1, &angledBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Back button
        RECT backBtn = { 30, bottom - 80, 130, bottom - 31 };
        DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
    }

} // namespace Drawing
